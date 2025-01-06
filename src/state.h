using namespace std;
using Eigen::VectorXf;

class State {
    private:
        float yaw;
        float angular_speed;
        float linear_speed;
        float last_nonzero_ls;
        float last_nonzero_as;
        float lifting_height;
        float desp_max;
        float phi_max;

        int _cycle_length;
        int _cycle_state;
        bool _soft_stop_cycle;

        int _stages;

        Eigen::MatrixXf coords;
        VectorXf gait_offsets;
        VectorXf steady_state;

    public:
        State(float lifting_height, float desp_max, float phi_max);
        void set_yaw(float yaw);
        void set_linear_speed(float linear_speed);
        void set_angular_speed(float angular_speed);
        void set_steady_state(const vector<float>& crds);
        void _adjust_cycle();
        Eigen::VectorXf _get_target_coords(int offset);
        pair<vector<vector<float>>, vector<vector<int>>> next_state();
};

State::State(float lifting_height = -2.5, float desp_max = 6.0, float phi_max = radians(13)) {
    this->lifting_height = lifting_height;
    this->desp_max = desp_max;
    this->phi_max = phi_max;
    this->yaw = 0.0;
    this->angular_speed = 0.0;
    this->linear_speed = 0.0;
    this->last_nonzero_ls = 0.0;
    this->last_nonzero_as = 0.0;

    this->_cycle_length = 0; // period is cycle_length * refresh_rate = (N * 0.025)
    this->_cycle_state = 0;
    this->_soft_stop_cycle = false;

    this->_stages = 3;

    this->coords = Eigen::MatrixXf::Zero(6,6);

    this->gait_offsets.resize(6);
    this->gait_offsets << 1.0, 0.0, 2.0, 1.0, 0.0, 2.0;
    this->steady_state.setZero(6);
}

void State::set_yaw(float yaw) {
    if(this->linear_speed > 1e-3) { // default position
        this->yaw = yaw;
        printf("%f\n", this->yaw);
    }
}

void State::set_linear_speed(float linear_speed) {
    if(linear_speed >= 1e-3) this->last_nonzero_ls = linear_speed;
    this->linear_speed = linear_speed;
    printf("%f\n", this->linear_speed);
    this->_adjust_cycle();
}

void State::set_angular_speed(float angular_speed){
    if(abs(angular_speed) > 1e-3) this->last_nonzero_as = angular_speed;
    this->angular_speed = angular_speed;
    printf("%f\n", this->angular_speed);
    this->_adjust_cycle();
}

void State::set_steady_state(const vector<float>& crds) {
    if(crds.size() == 1) { // reset
        this->steady_state.setZero();
        for(int i=0; i<6; i++) Serial.printf("%f ", this->steady_state(i));
        Serial.println();
    }
    else {
        for(int i=0; i<crds.size(); i++) {
            if(crds[i] != 0.) this->steady_state(i) = crds[i];
            Serial.printf("%f ", crds[i]);
        }
        Serial.println();
    }
}

void State::_adjust_cycle(){
    int N_linear, N_angular;
    if(abs(this->linear_speed) < 1e-3)  N_linear = 0;
    else N_linear = round(this->_stages * this->desp_max / ((this->_stages - 1) * abs(this->linear_speed)));
    if(abs(this->angular_speed) < 1e-3) N_angular = 0;
    else N_angular = round(2 * this->_stages * this->phi_max / ((this->_stages - 1) * abs(this->angular_speed)));
    int prev_cycle_length = this->_cycle_length;

    if(N_linear == 0 && N_angular == 0)
        this->_soft_stop_cycle = true;
    else if(N_angular == 0)
        this->_cycle_length = N_linear;
    else if(N_linear == 0)
        this->_cycle_length = N_angular;
    else
        this->_cycle_length = min(N_linear, N_angular);

    if(prev_cycle_length > 0)
        this->_cycle_state = round(this->_cycle_length * this->_cycle_state / prev_cycle_length);
}

VectorXf State::_get_target_coords(int offset){
    if(this->_cycle_length == 0) return VectorXf::Zero(6);
    int sample_point = (this->_cycle_state + offset) % this->_cycle_length;
    float d_xz = (this->_stages - 1) * this->linear_speed * this->_cycle_length / this->_stages;
    int sign_multiplier;
    float y;
    if(sample_point < this->_cycle_length / this->_stages) {
        sign_multiplier = -1;
        y = this->lifting_height * sin(M_PI * this->_stages * sample_point / this->_cycle_length);
    }
    else {
        sign_multiplier = 1;
        y = 0.0;
    }

    VectorXf coords(6);

    coords << sign_multiplier * d_xz / 2 * sin(yaw),       // x
                y,                                            // y
                sign_multiplier * d_xz / 2 * cos(yaw),   // z
                0.0,                                          // th
                sign_multiplier * (float(_stages) - 1) * angular_speed * _cycle_length / (2 * float(_stages)), // phi
                0.0;   //psi
    
    return coords;

}

pair<vector<vector<float>>, vector<vector<int>>> State::next_state() {
    vector<vector<float>> movs;
    vector<vector<int>> targets;

    int offset;
    VectorXf target_coords, D_hat, diff;
    for(int i=0; i<6; i++){
        offset = int(this->gait_offsets(i) / this->_stages * this->_cycle_length);
        target_coords = this->_get_target_coords(offset);
        diff = target_coords - this->coords.row(i).transpose();
        D_hat = diff.array() / (diff.array() + 1e-9).abs();

        float upper_bound_x, upper_bound_z, upper_bound_phi;
        if(abs(target_coords(0)) < 1e-3 && abs(target_coords(0) - this->coords(i, 0)) > abs(this->last_nonzero_ls * sin(this->yaw))) upper_bound_x = abs(this->coords(i,0));
        else upper_bound_x = abs(target_coords(0));
        if(abs(target_coords(2)) < 1e-3 && abs(target_coords(2) - this->coords(i, 2)) > abs(this->last_nonzero_ls * cos(this->yaw))) upper_bound_z = abs(this->coords(i, 2));
        else upper_bound_z = abs(target_coords(2));
        if(abs(target_coords(4)) < 1e-3 && abs(target_coords(4) - this->coords(i, 4)) > abs(this->last_nonzero_as)) upper_bound_phi = abs(this->coords(i, 4));
        else upper_bound_phi = abs(target_coords(4));

        float speed_x = abs(this->last_nonzero_ls * sin(this->yaw));
        float speed_z = abs(this->last_nonzero_ls * cos(this->yaw));
        float speed_phi = abs(this->last_nonzero_as);
        if(this->_cycle_length > 0 && (this->_cycle_state + offset) % this->_cycle_length < this->_cycle_length / this->_stages){
            speed_x *= (this->_stages - 1);
            speed_z *= (this->_stages - 1);
            speed_phi *= (this->_stages - 1);
        }

        this->coords(i, 0) = min(max(this->coords(i, 0) + speed_x * D_hat(0), -upper_bound_x), upper_bound_x);
        this->coords(i, 1) = target_coords(1);
        this->coords(i, 2) = min(max(this->coords(i, 2) + speed_z * D_hat(2), -upper_bound_z), upper_bound_z);
        this->coords(i, 4) = min(max(this->coords(i, 4) + speed_phi * D_hat(4), -upper_bound_phi), upper_bound_phi);

        // if(i == 0)
        //     printf("%f %f %f %d\n", this->coords(i, 1), target_coords(2), this->coords(i, 2), this->_cycle_length);
    }

    // Avanza en ciclo si existe ciclo
    // Serial.printf("%d\n", this->_cycle_length);
    if(this->_cycle_length > 0){
        this->_cycle_state = (this->_cycle_state + 1) % this->_cycle_length;
        // la detencion solo es posible si xz esta en el target 0
        float maxX = this->coords.col(0).maxCoeff();
        float maxZ = this->coords.col(2).maxCoeff();
        printf("%d - ", this->_soft_stop_cycle);
        printf("%d - ", this->_cycle_state);
        printf("%f\n", maxX * maxX + maxZ * maxZ);
        if(this->_soft_stop_cycle && this->_cycle_state == 0 && sqrt(maxX * maxX + maxZ * maxZ) < 1e-3) {
            this->_cycle_state = 0;
            this->_cycle_length = 0;
            this->_soft_stop_cycle = false;
        }
    }

    for(int i=0; i<6; i++) {
        VectorXf combined = this->coords.row(i).transpose() + this->steady_state;

        movs.push_back(vector<float>(combined.data(), combined.data() + combined.size()));

        targets.push_back({0,0,0,0,0,0});
        targets[i][i] = 1;
    }

    return make_pair(movs, targets);
}