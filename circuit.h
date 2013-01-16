//created: 05/11/12
//author: Simon Rankine
//updated: 03/01/13

class circuit{
	public:
		static bool regulator(float * port2, bool front);
		static bool divider(int * R1,int * R2, bool front);
		static bool delta(int * R2, int * R3, bool front);
		static bool star(int * R1, int * R2, bool front);
		static bool highPass(int * R1, float * C1, int * corner, bool front);
		static bool lowPass(int * R1, float * C1, int * corner, bool front);
		static bool bandPass(int * R1, float * C1, int * resonance, bool front);
		static bool testFunction(int *a, int *b, int *c, int *d);
		static bool disengauge();
	private:
		static float readR(int Rpin, int Gpin, bool noZ = false, bool low = false);
		static float readC(int Rpin, int Gpin);
		static float readVoltage(int pin);
		static void caCon(int pin);
		static void DC(int pin);
		static void fiveV(int pin);
		static void capR(int pin);
		static void ground(int pin);
		static void highR(int pin);
		static void lowR(int pin);
		static void highZ(int pin);
		static int	toR24(int R);
		static float	toC12(int C);
		static float	conform(int Z, float E[], int max);
};

