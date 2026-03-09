#include "pso_rastrigin.h"
#include "pso_rosenbrock.h"
#include "cs_rastrigin.h"
#include "cs_rosenbrock.h"
#include "gwo_rastrigin.h"
#include "gwo_rosenbrock.h"
#include "sga_rastrigin.h"
#include "sga_rosenbrock.h"
#include "abc_rastrigin.h"
#include "abc_rosenbrock.h"

int main()
{
	//pso::run_rastrigin_experiments();
	//pso::run_rosenbrock_experiments();
	//cs::run_rastrigin_experiments();
	//cs::run_rosenbrock_experiments();
	//gwo::run_rastrigin_experiments();
	//gwo::run_rosenbrock_experiments();
	//sga::run_rastrigin_experiments();
	//sga::run_rosenbrock_experiments();
	//abc::run_rastrigin_experiments();
	abc::run_rosenbrock_experiments();

	return 0;
}