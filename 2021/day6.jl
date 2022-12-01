### A Pluto.jl notebook ###
# v0.17.7

using Markdown
using InteractiveUtils

# ╔═╡ c0581a96-a7a5-11ec-3b35-e5130814216a
md"""
# Day 6: Lanternfish #
## Problem 1 ##

The sea floor is getting steeper. Maybe the sleigh keys got carried this way?

A massive school of glowing [lanternfish](https://en.wikipedia.org/wiki/Lanternfish) swims past. They must spawn quickly to reach such large numbers - maybe **exponentially** quickly? You should model their growth rate to be sure.

Although you know nothing about this specific species of lanternfish, you make some guesses about their attributes. Surely, each lanternfish creates a new lanternfish once every 7 days.

However, this process isn't necessarily synchronized between every lanternfish - one lanternfish might have 2 days left until it creates another lanternfish, while another might have 4. So, you can model each fish as a single number that represents **the number of days until it creates a new lanternfish**.

Furthermore, you reason, a **new** lanternfish would surely need slightly longer before it's capable of producing more lanternfish: two more days for its first cycle.

So, suppose you have a lanternfish with an internal timer value of 3:

- After one day, its internal timer would become 2.
- After another day, its internal timer would become 1.
- After another day, its internal timer would become 0.
- After another day, its internal timer would reset to 6, and it would create a **new** lanternfish with an internal timer of 8.
- After another day, the first lanternfish would have an internal timer of 5, and the second lanternfish would have an internal timer of 7.

A lanternfish that creates a new fish resets its timer to 6, **not 7** (because 0 is included as a valid timer value). The new lanternfish starts with an internal timer of 8 and does not start counting down until the next day.

Realizing what you're trying to do, the submarine automatically produces a list of the ages of several hundred nearby lanternfish (your puzzle input). For example, suppose you were given the following list:

3,4,3,1,2

This list means that the first fish has an internal timer of 3, the second fish has an internal timer of 4, and so on until the fifth fish, which has an internal timer of 2. Simulating these fish over several days would proceed as follows:
```
Initial state: 3,4,3,1,2
After  1 day:  2,3,2,0,1
After  2 days: 1,2,1,6,0,8
After  3 days: 0,1,0,5,6,7,8
After  4 days: 6,0,6,4,5,6,7,8,8
After  5 days: 5,6,5,3,4,5,6,7,7,8
After  6 days: 4,5,4,2,3,4,5,6,6,7
After  7 days: 3,4,3,1,2,3,4,5,5,6
After  8 days: 2,3,2,0,1,2,3,4,4,5
After  9 days: 1,2,1,6,0,1,2,3,3,4,8
After 10 days: 0,1,0,5,6,0,1,2,2,3,7,8
After 11 days: 6,0,6,4,5,6,0,1,1,2,6,7,8,8,8
After 12 days: 5,6,5,3,4,5,6,0,0,1,5,6,7,7,7,8,8
After 13 days: 4,5,4,2,3,4,5,6,6,0,4,5,6,6,6,7,7,8,8
After 14 days: 3,4,3,1,2,3,4,5,5,6,3,4,5,5,5,6,6,7,7,8
After 15 days: 2,3,2,0,1,2,3,4,4,5,2,3,4,4,4,5,5,6,6,7
After 16 days: 1,2,1,6,0,1,2,3,3,4,1,2,3,3,3,4,4,5,5,6,8
After 17 days: 0,1,0,5,6,0,1,2,2,3,0,1,2,2,2,3,3,4,4,5,7,8
After 18 days: 6,0,6,4,5,6,0,1,1,2,6,0,1,1,1,2,2,3,3,4,6,7,8,8,8,8
```
Each day, a 0 becomes a 6 and adds a new 8 to the end of the list, while each other number decreases by 1 if it was present at the start of the day.

In this example, after 18 days, there are a total of 26 fish. After 80 days, there would be a total of **5934**.

Find a way to simulate lanternfish. **How many lanternfish would there be after 80 days**?
"""

# ╔═╡ 21076948-b3d6-429b-9789-0ece1ac4b747
md"""
## Solution 1 ##
This should(!!) be relatively simple.  Set up an array with the inital values then at each generation decrease every value by one.  When a value gets to -1 it spawns so count the number of -1's and add that many 8's to the end of the array.  Reset every -1 to 6 then continue with the next generation.
"""

# ╔═╡ 3ee685b8-6210-4419-b427-82ccdbb9cb3e
problem_data = readline("data/lantern_fish.dat");

# ╔═╡ ae5cd1b5-426c-4432-90a6-914dfee8aa78
initial_generation = [parse(Int, i) for i in split(problem_data, ",")]
# initial_generation = [3,4,3,1,2]

# ╔═╡ fff0345f-0ee2-47b4-a9e0-b2d97a3dc9cc
function do_generations(old_generation, cycles)
	while cycles > 0
		old_generation = broadcast(-, old_generation, 1)
		new_births  = fill(8, count(i -> (i == -1), old_generation))
		old_generation[old_generation .== -1] .= 6
#=		for n ∈ new_births
			push!(old_generation, n)
		end
=#
		append!(old_generation, new_births)
		cycles -= 1
	end
	return old_generation
end;

# ╔═╡ 7ba2ed20-7023-4993-ae31-6bbf18799571
final_generation = do_generations(initial_generation, 80);

# ╔═╡ dd77054c-d149-4b55-bc61-24b67ca682c3
initial_generation

# ╔═╡ c73bd3a3-4a78-4e2a-b0a0-32d8b53f5b99
md"""There will be $(size(final_generation, 1)) lantern fish after 80 days """

# ╔═╡ df7da8a9-b995-4d89-a8e1-a316a3ea13ad
md"""
## Part Two ##

Suppose the lanternfish live forever and have unlimited food and space. Would they take over the entire ocean?

After 256 days in the example above, there would be a total of **26984457539** lanternfish!

**How many lanternfish would there be after 256 days?**

"""

# ╔═╡ a39408bd-922f-4ecb-8c60-1ece035dd2d3
md"""
## Solution 2 ##
---

Now things become a bit trickier as the above solution is not going to work for a large number of generations.  The algorythm is of the ``O^N`` so will rapidly blow up for large numbers of generations.

An alternate method which sould be a linear solution is to treat it as a 9 day vector that can be rotated and the items at day 1 get rotated to day 9 as new births and added to day 7 as mature lampreys ready to give birth again.

"""

# ╔═╡ 3ecb364e-5448-4ce9-9e6f-b7fecba47485
function tally(items, span)
	ret = zeros(Int, span)
	for n in sort(items)
		ret[n] += 1
	end
	return ret
end

# ╔═╡ aeb5d423-8ac9-431b-9466-e6a742cd34cc
generation_vector = tally(initial_generation, 9)

# ╔═╡ de1fdfcd-7771-40bc-aaec-cd04c3b414f3
function do_vector_generations(start_vector, cycles)
	res_vector = []
	copy!(res_vector, start_vector)
	for x in 1:(cycles-1)
		res_vector = circshift(res_vector, -1)
		res_vector[7] += res_vector[9]
	end
	return res_vector
end

# ╔═╡ 3afff0e4-4fcb-4cd1-9b8c-63e1628e8662
end_vector = do_vector_generations(generation_vector, 256)

# ╔═╡ f8540762-2c91-4f36-ab02-d60307851f63
md"""There will be $(sum(end_vector)) lantern fish after after 256 days """

# ╔═╡ 00000000-0000-0000-0000-000000000001
PLUTO_PROJECT_TOML_CONTENTS = """
[deps]
"""

# ╔═╡ 00000000-0000-0000-0000-000000000002
PLUTO_MANIFEST_TOML_CONTENTS = """
# This file is machine-generated - editing it directly is not advised

julia_version = "1.7.2"
manifest_format = "2.0"

[deps]
"""

# ╔═╡ Cell order:
# ╟─c0581a96-a7a5-11ec-3b35-e5130814216a
# ╟─21076948-b3d6-429b-9789-0ece1ac4b747
# ╠═3ee685b8-6210-4419-b427-82ccdbb9cb3e
# ╠═ae5cd1b5-426c-4432-90a6-914dfee8aa78
# ╠═fff0345f-0ee2-47b4-a9e0-b2d97a3dc9cc
# ╠═7ba2ed20-7023-4993-ae31-6bbf18799571
# ╠═dd77054c-d149-4b55-bc61-24b67ca682c3
# ╠═c73bd3a3-4a78-4e2a-b0a0-32d8b53f5b99
# ╟─df7da8a9-b995-4d89-a8e1-a316a3ea13ad
# ╟─a39408bd-922f-4ecb-8c60-1ece035dd2d3
# ╠═3ecb364e-5448-4ce9-9e6f-b7fecba47485
# ╠═aeb5d423-8ac9-431b-9466-e6a742cd34cc
# ╠═de1fdfcd-7771-40bc-aaec-cd04c3b414f3
# ╠═3afff0e4-4fcb-4cd1-9b8c-63e1628e8662
# ╟─f8540762-2c91-4f36-ab02-d60307851f63
# ╟─00000000-0000-0000-0000-000000000001
# ╟─00000000-0000-0000-0000-000000000002
