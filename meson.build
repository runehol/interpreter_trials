project('interpreter_trials', 'cpp',
							  default_options : ['cpp_std=c++17'])

benchmark_proj = subproject('google-benchmark')
benchmark_dep = benchmark_proj.get_variable('google_benchmark_dep')

executable('interpreter_trials',
	'interpreter_trials.cpp',
	'bytecode.cpp',
	'interpreter1.cpp',
	'interpreter2.cpp',
	'interpreter3.cpp',
	'interpreter4.cpp',
	'interpreter5.cpp',
	'interpreter6.cpp',
	'interpreter7.cpp',
					dependencies : benchmark_dep)
