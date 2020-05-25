int min(int a, int b);


int min_period(int * set, int rows);


void gen_cri_sen(int * set, int rows, int sem, int cri_sen[rows][sem]);


int* gen_ceil(int rows, int columns, int cri_sen[rows][columns]);


int * calc_b(int * set, int n_tasks, int n_sem);