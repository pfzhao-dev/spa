import sys


def process_file(filename):
    f = open(filename)
    lines = f.readlines()

    num_tc = 0
    total_time = 0

    for line in lines:
        if not line.startswith("<time_taken>"):
            continue

        s = line[12:]
        s = s[:-14]
        v = float(s)

        num_tc += 1
        total_time += v

    f.close()

    print('---')
    print('File: ', filename)
    print('>> num_tc: ', num_tc, ' total_time: ', total_time, ' avg_time: ', total_time / num_tc)
    print('---')


if __name__ == "__main__":
    filenames = sys.argv[1:]

    for filename in filenames:
        process_file(filename)
