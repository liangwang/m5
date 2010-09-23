# running specified programs
parser.add_option("-c", "--cmd",
        default=joinpath(m5_root,
            "tests/test-progs/hello/bin/alpha/linux/hello"),
        help="The binary to run in syscall emulation mode.")
parser.add_option("-o", "--options", default="",
        help='The options to pass to the binary, use " " around the entire string')

# running splash2 programs
parser.add_option("--rootdir",
                  help='Root directory of Splash2',
                  default=joinpath(m5_root, "../se_splash2/splash2/codes"))
parser.add_option("-b", "--benchmark",
                  help="Splash2 benchmark to run")

# O3CPU options
DEFAULT_WIDTH=2
parser.add_option("--num-cpus", dest="num_cpus", type = "int",
        default = 1, help = 'number of cpus')
parser.add_option('--numThreads', default=1, type='int',
        help='number of threads')
parser.add_option("--MATEntries", dest="MATEntries", type="int",
        default = 32, help="Number of entries in MAT")

parser.add_option("--LQEntries", dest="LQEntries", type="int",
                  default=192, help="Number of entries in Load queue")
parser.add_option("--SQEntries", dest="SQEntries", type="int",
                  default=192, help="Number of entries in Store queue")


parser.add_option("--numSubscribers", type="int", default=2,
        help="Max number of subscribers")
parser.add_option("--pipeWidth", type="int", default=DEFAULT_WIDTH,
        help="Max number of pipeline width")

parser.add_option("--fetchWidth", dest="fetchWidth", type="int",
                  default=DEFAULT_WIDTH,
        help="fetch width")
parser.add_option("--decodeWidth", dest="decodeWidth", type="int", default=DEFAULT_WIDTH,
        help="decode width")
parser.add_option("--renameWidth", dest="renameWidth", type="int", default=DEFAULT_WIDTH,
        help="rename width")
parser.add_option("--dispatchWidth", dest="dispatchWidth", type="int", default=DEFAULT_WIDTH,
        help="dispatch width")
parser.add_option("--issueWidth", dest="issueWidth", type="int", default=DEFAULT_WIDTH,
        help="issue width")
parser.add_option("--wbWidth", dest="wbWidth", type="int", default=DEFAULT_WIDTH,
        help="write back width")
parser.add_option("--commitWidth", dest="commitWidth", type="int", default=DEFAULT_WIDTH,
        help="commit width")
parser.add_option("--squashWidth", dest="squashWidth", type="int", default=DEFAULT_WIDTH,
        help="squash width")
