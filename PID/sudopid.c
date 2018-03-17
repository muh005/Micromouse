PID Angular;
void init_PID(Number Kp, Number Ki, Number Kd, Number Exp, Number Act){
	Angular.Kp = Kp;
	Angular.Ki = Ki;
	Angular.Kd = Kd;
	Angular.preExp = Exp;
	Angular.preAct = Act;
	Angular.integral = 0;
	Angular.P = Kp * (Exp - Act);
	Angular.I = 0;
	Angular.D = 0;
	Angular.T = now();
}

Number update_PID(Number Exp, Number Act){
	Number dT = (now() - Angular.T)/ 1000000;//convert into microsecond
	Number curError = Exp - Act;
	Number preError = Angular.preExp - Angular.preAct;
	//--------P--------//
	Number P = Angular.Kp * curError;

	//initial state//
	//let timeinterval be 0.01//
	if(dT > timeinterval){
		Angular.preExp = Exp;
		Angular.preAct = Act;
		Angular.T = now();
		return P + Angular.I + Angular.D;
	}

	//-------I--------//
	//if expected value change, reset integral//
	if(Angular.preExp != Exp) Angular.integral = 0;

	//otherwise integrate error//
	Angular.integral += (curError + preError)/2 * dT;
	Number I = Angular.Ki * Angular.integral;

	//-------D--------//
	Number D;
	//if no error//
	if(Act == Angular.preAct) D = 0;
	
	//otherwise fix it//
	else{
		Angular.differential = (Angular.preAct - Act)/ dT;
		D = Angular.differential * Angular.Kd;
	}

	//----update----//
	Angular.preExp = Exp;
	Angular.preAct = Act;
	Angular.T = now();

	//----return----//
	Angular.P = P; Angular.I = I; Angular.D = D;
	return (P + I + D);
}
