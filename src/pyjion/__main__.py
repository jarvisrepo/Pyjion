import runpy
import pyjion
import argparse


def main():
    parser = argparse.ArgumentParser(prog='pyjion', description='Python JIT Compiler')

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('script', help='script file', nargs='?')

    group.add_argument('-m', action='store', type=str, dest="module", help="Execute module")

    parser.add_argument('--tracing',
                        action='store_true',
                        help='Enable tracing (used by debuggers)')

    parser.add_argument('--profiling',
                        action='store_true',
                        help='Enable profiling')

    parser.add_argument('--graph',
                        action='store_true',
                        help='Enable instruction graph generation')

    parser.add_argument('--debug',
                        action='store_true',
                        help='Enable debuggable JIT methods')

    parser.add_argument('--no-pgc',
                        action='store_true',
                        help='Disable PGC')

    parser.add_argument('-o',
                    '--opt-level',
                    action='store', type=int, default=1,
                    help='Optimization level (default 1')

    args = parser.parse_args()

    pyjion.enable()

    if args.tracing:
        pyjion.enable_tracing()
    if args.profiling:
        pyjion.enable_profiling()
    if args.graph:
        pyjion.enable_graphs()
    if args.debug:
        pyjion.enable_debug()
    if args.no_pgc:
        pyjion.disable_pgc()

    pyjion.set_optimization_level(args.opt_level)

    if args.module:
        runpy.run_module(args.module)
    else:
        runpy.run_path(args.script)
    pyjion.disable()


if __name__ == "__main__":
    main()
