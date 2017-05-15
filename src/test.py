from random import randint
from math import floor
from sys import argv

def test_dist(area, n_colors):
    vals = []
    total = 0
    
    rem = area
    half = floor(area / 2)
    for i in range(0, n_colors - 1):
        n_colors_left = n_colors - i
        hi = min(half, rem - n_colors_left + 1)
        lo = rem // n_colors_left

        val = randint(lo, hi)
        #print('{} (max: {} min: {})'.format(val, hi, lo))

        vals.append(val)
        rem -= val
        total += val

    vals.append(rem)
    total += rem

    bad_total = total != area
    bad_val = max(vals) > half
    fail = bad_total or bad_val
    print('{}ed on {}'.format('fail' if fail else 'pass', vals), end=' ')
    if fail:
        print('because', end=' ')
        if bad_total:
            print('sum does not equal area')
        if bad_val:
            print('a count exceeded half')
    print()

if __name__ == '__main__':
    s = set()
    for i in range(0, int(argv[1]) + 1):
        for j in range(0, int(argv[2]) + 1):
            area = i * j
            if area not in s:
                s.add(area)
                for k in range(2, area + 1):
                    for l in range(0, 1):
                        if area % 2 == 0 or k != 2:
                            print('{} x {} (area {:3}), {:3} colors'.format(i, j, area, k), end=' ')
                            test_dist(area, k)

