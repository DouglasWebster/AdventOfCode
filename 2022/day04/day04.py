import time

start = time.perf_counter()
with open("input.txt", "r") as f:
  total_part1 = 0
  total_part2 = 0
  for pair, assignment in enumerate(f.read().splitlines()):
      assignment_per_pair = []
      for elf_assign in assignment.split(","):
          sections_delimiter = elf_assign.split("-")
          assignment_per_pair.append({section for section in range(int(sections_delimiter[0]), int(sections_delimiter[-1]) + 1)})
      if assignment_per_pair[0].issubset(assignment_per_pair[1]) or assignment_per_pair[0].issuperset(assignment_per_pair[1]):
          total_part1 += 1
      if len(assignment_per_pair[0] & assignment_per_pair[1]) != 0:
          total_part2 += 1
  print(total_part1, total_part2)
  print((time.perf_counter() - start) * 1000)
