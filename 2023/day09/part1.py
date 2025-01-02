
from sys import argv as av

FILE = 'input' if len(av) != 2 else av[1]

with open(FILE) as file:
    lines = [[int(num) for num in line.split()] for line in file.readlines()]

def recursive(nums: list[int]) -> int:
    diffs: list[int] = []
    zeros = 0
    i = 1
    while i < len(nums):
        diffs.append(nums[i] - nums[i - 1])
        zeros += (diffs[-1] == 0)
        i += 1

    if zeros == len(diffs):
        return nums[-1]

    return recursive(diffs) + nums[-1]

count = 0
for line in lines:
    count += recursive(line)

print(count)
