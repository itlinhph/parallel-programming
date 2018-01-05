
#ifndef INIT_VALUE
#define INIT_VALUE 25.0
#endif

#ifndef INIT_HEAD
#define INIT_HEAD 100.0
#endif

void FD(float* C, float* dC, float D, float dx, int m) {
	int i;
	for (i=0; i<m; i++) {
		float c = C[i];
		// Get adjacent values unless boundary is reached
		float u = (i==0) ? INIT_HEAD : C[i-1]; 
		float d = (i==m-1) ? INIT_VALUE : C[i+1];

		// Update value
		dC[i] = (D / (dx * dx)) * (u + d - 2 * c);
	}
}

void solve_single(float* C, float* dC, float dx, int m, float T, float dt, float D) {
	float t;
	int i;

	for (t=0.0; t<=T; t+=dt) {
		FD(C, dC, D, dx, m);
		for (i=0; i<m; i++) {
			C[i] = C[i] + dt * dC[i];
		}
	}
}
