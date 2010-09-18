# ------------
# Define Splash2 Benchmarks
#============

class Cholesky(LiveProcess):
    cwd = options.rootdir + '/kernels/cholesky'
    executable = options.rootdir + '/kernels/cholesky/CHOLESKY'
    cmd = ['CHOLESKY', '-p' + str(options.num_cpus),
           options.rootdir + '/kernels/cholesky/inputs/tk23.0']

class FFT(LiveProcess):
    cwd = options.rootdir + '/kernels/fft'
    executable = options.rootdir + '/kernels/fft/FFT'
    cmd = ['FFT', '-p', str(options.num_cpus), '-m18']

class LU_contig(LiveProcess):
    executable = options.rootdir + '/kernels/lu/contiguous_blocks/LU'
    cmd = ['LU', '-p', str(options.num_cpus)]
    cwd = options.rootdir + '/kernels/lu/contiguous_blocks'

class LU_noncontig(LiveProcess):
    executable = options.rootdir + '/kernels/lu/non_contiguous_blocks/LU'
    cmd = ['LU', '-p', str(options.num_cpus)]
    cwd = options.rootdir + '/kernels/lu/non_contiguous_blocks'

class Radix(LiveProcess):
    executable = options.rootdir + '/kernels/radix/RADIX'
    cmd = ['RADIX', '-n524288', '-p', str(options.num_cpus)]
    cwd = options.rootdir + '/kernels/radix'

class Barnes(LiveProcess):
    executable = options.rootdir + '/apps/barnes/BARNES'
    cmd = ['BARNES']
    input = options.rootdir + '/apps/barnes/input.p' + str(options.num_cpus)
    cwd = options.rootdir + '/apps/barnes'

class FMM(LiveProcess):
    executable = options.rootdir + '/apps/fmm/FMM'
    cmd = ['FMM']
    if str(options.num_cpus) == '1':
        input = options.rootdir + '/apps/fmm/inputs/input.2048'
    else:
        input = options.rootdir + '/apps/fmm/inputs/input.2048.p' + str(options.num_cpus)
        cwd = options.rootdir + '/apps/fmm'

class Ocean_contig(LiveProcess):
    executable = options.rootdir + '/apps/ocean/contiguous_partitions/OCEAN'
    cmd = ['OCEAN', '-p', str(options.num_cpus)]
    cwd = options.rootdir + '/apps/ocean/contiguous_partitions'

class Ocean_noncontig(LiveProcess):
    executable = options.rootdir + '/apps/ocean/non_contiguous_partitions/OCEAN'
    cmd = ['OCEAN', '-p', str(options.num_cpus)]
    cwd = options.rootdir + '/apps/ocean/non_contiguous_partitions'

class Raytrace(LiveProcess):
    executable = options.rootdir + '/apps/raytrace/RAYTRACE'
    cmd = ['RAYTRACE', '-p' + str(options.num_cpus),
           options.rootdir + '/apps/raytrace/inputs/teapot.env']
    cwd = options.rootdir + '/apps/raytrace'

class Water_nsquared(LiveProcess):
    executable = options.rootdir + '/apps/water-nsquared/WATER-NSQUARED'
    cmd = ['WATER-NSQUARED']
    if options.num_cpus==1:
        input = options.rootdir + '/apps/water-nsquared/input'
    else:
        input = options.rootdir + '/apps/water-nsquared/input.p' + str(options.num_cpus)
        cwd = options.rootdir + '/apps/water-nsquared'

class Water_spatial(LiveProcess):
    executable = options.rootdir + '/apps/water-spatial/WATER-SPATIAL'
    cmd = ['WATER-SPATIAL']
    if options.num_cpus==1:
        input = options.rootdir + '/apps/water-spatial/input'
    else:
        input = options.rootdir + '/apps/water-spatial/input.p' + str(options.num_cpus)
        cwd = options.rootdir + '/apps/water-spatial'
