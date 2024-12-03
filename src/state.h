using namespace std;
using Eigen::Vector3f;

class State {
    private:
        float yaw;
        float interval;
        float last_nonzero_interval;
        float amplitude;
        float lifting_height;

        bool phase;
        Vector3f P_A;
        Vector3f P_B;

        Eigen::VectorXf steady_state;
    public:
        State(float yaw, float interval, float lifting_height);
        void set_yaw(float yaw);
        void set_speed(float interval);
        void set_steady_state(const vector<float>& crds);
        float _compute_y(float pbx, float pbz, float offset_x, float offset_z);
        pair<vector<vector<float>>, vector<vector<int>>> soft_next_state();
};

State::State(float yaw = 0.0, float interval = 0., float lifting_height = -2.5) {
    this->yaw = yaw;
    this->interval = interval;
    this->last_nonzero_interval = interval;
    this->amplitude = 6;
    this->lifting_height = lifting_height;

    this->phase = true;
    this->P_A.setZero();
    this->P_B = -this->P_A;

    this->steady_state.setZero(6);
}

void State::set_yaw(float yaw) {
    this->yaw = yaw;
    printf("%f\n", this->yaw);
}

void State::set_speed(float interval) {
    if(interval >= 1e-3) this->last_nonzero_interval = interval;
    this->interval = interval;
    printf("%f\n", this->interval);
    this->amplitude = 6;
}

void State::set_steady_state(const vector<float>& crds) {
    for(int i=0; i<crds.size(); i++) {
        if(crds[i] != 0.) this->steady_state(i) = crds[i];
        Serial.printf("%f ", crds[i]);
    }
    Serial.println();
}
        
float State::_compute_y(float pbx, float pbz, float offset_x, float offset_z) {
    return 2 * this->lifting_height / this->amplitude * sqrt( abs( pow(this->amplitude, 2) / 4 - pow((pbx - offset_x), 2) - pow((pbz - offset_z), 2)));
}

pair<vector<vector<float>>, vector<vector<int>>> State::soft_next_state() {
    vector<vector<float>> movs;
    vector<vector<int>> targets;

    // If there is no movement and the distance to the origen is 0, save calculations :p
    if(this->interval < 1e-3 && this->P_B.norm() < 1e-3) {
        movs.resize(1);
        for(int i=0; i<this->steady_state.size(); i++) movs[0].push_back(this->steady_state(i));
        targets = {{1, 1, 1, 1, 1, 1}};
        return make_pair(movs, targets);
    }
        // return ([[0, 0, 0, 0, 0, 0], [1, 1, 1, 1, 1, 1]])
    
    Vector3f target_A, target_B, D_hat_A, D_hat_B;
    float mirror_interval, dist2d, rho;

    if(this->interval < 1e-3) target_A.setConstant(0.0); // Objetivo: origen (detencion)
    else target_A << (this->amplitude / 2 * sin(this->yaw)), 0, (this->amplitude / 2 * cos(this->yaw)); // Objetivo: semiamplitud (cambio de fase)

    target_B = -target_A;
    D_hat_A = (target_A - this->P_A) / ((target_A - this->P_A).norm() + 1e-9); // Direccion unitaria grupo A
    D_hat_B = -D_hat_A;

    // Distancia al objetivo
    dist2d = (this->P_A - target_A).norm();

    if(this->interval < 1e-3){
        // Detencion: se dirige al origen
        if(dist2d <= this->last_nonzero_interval){
            this->P_A = target_A;
            this->P_B = target_A;
        }
        else{
            this->P_A += this->last_nonzero_interval * D_hat_A;
            this->P_B += this->last_nonzero_interval * D_hat_B;
        }
    }
    else if(dist2d <= this->interval){
        // Cambio de fase y reinicio de objetivos
        this->phase = !this->phase;
        mirror_interval = this->interval - dist2d;
        this->P_A = target_B + mirror_interval * D_hat_A;
        this->P_B = target_A + mirror_interval * D_hat_B;
    }
    else {
        // Caso normal: avance hacia el objetivo
        this->P_A += this->interval * D_hat_A;
        this->P_B += this->interval * D_hat_B;
    }

    // Calcula altura de recogimiento
    float target_B_y;
    if(this->interval < 1e-3){
        float P_B_norm = this->P_B(Eigen::Vector2f(0, 2)).norm();
        rho = this->amplitude / (2 * P_B_norm + 1e-9);
        if(P_B_norm < 1e-3)
            target_B_y = _compute_y(this->P_B(0), this->P_B(2), this->amplitude / 2, 0.);
        else
            target_B_y = _compute_y(this->P_B(0), this->P_B(2), rho * this->P_B(0), rho * this->P_B(2));
    }
    else
        target_B_y = this->_compute_y(this->P_B(0), this->P_B(2), 0., 0.);
    this->P_B(1) += min( max((target_B_y - this->P_B(1)), -this->last_nonzero_interval*2), this->last_nonzero_interval*2);

    // Obtiene movimientos para cada grupo de patas
    movs = {
        {this->P_A(0), this->P_A(1), this->P_A(2), 0, 0, 0}, 
        {this->P_B(0), this->P_B(1), this->P_B(2), 0, 0, 0}
    };
    for(int i=0; i<movs[0].size(); i++) {
        movs[0][i] += this->steady_state(i);
        movs[1][i] += this->steady_state(i);
    }
    targets = {
        {!this->phase, this->phase, !this->phase, this->phase, !this->phase, this->phase}, 
        {this->phase, !this->phase, this->phase, !this->phase, this->phase, !this->phase}
    };

    return make_pair(movs, targets);
}