PID Angular;
void initAngular_PID(float32_t Kp, float32_t Ki, float32_t Kd, float32_t Exp, float32_t Act) {
	Angular.Kp = Kp;
	Angular.Ki = Ki;
	Angular.Kd = Kd;
	Angular.pExp = Exp;
	Angular.pAct = Act;
	Angular.integral = 0.000f;
	Angular.P = Kp * (Exp - Act); 
	Angular.I = 0.000f; 
	Angular.D = 0.000f;
	Angular.T = micros();
}
float32_t Angular_PID(float32_t Exp, float32_t Act) {
	//float32_t dT = (float32_t)(micros() - Angular.T) / 1000000.000f;
	//-------P--------//
	Angular.Ep = Exp - Act;
	float32_t P = Angular.Kp * Angular.Ep; 
	
	if(dT &gt; 0.01000f) {
		Angular.pExp = Exp;
		Angular.pAct = Act;
		Angular.T = micros();
		return P + Angular.I + Angular.D;
	}
	//-------I--------//
	if(Angular.pExp != Exp) Angular.integral = 0.000f;
		Angular.integral += (((Exp - Act) + (Angular.pExp - Angular.pAct)) / 2.0000000000f) * dT ;
		float32_t I = Angular.Ki * Angular.integral;
	//-------D--------//
	float32_t D;
	if(equal(Act, Angular.pAct, 0.000001)) D = 0.000f;
	else {
		Angular.differential = (Angular.pAct - Act) / (dT);
		D = Angular.differential * Angular.Kd;
	}
	//----update-----//
	Angular.pExp = Exp;
	Angular.pAct = Act;
	Angular.T = micros();
	//----return-----//
	Angular.P = P; Angular.I = I; Angular.D = D;
	return (P + I + D);
	return P;
}
