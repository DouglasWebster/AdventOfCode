### A Pluto.jl notebook ###
# v0.17.7

using Markdown
using InteractiveUtils

# ╔═╡ b81df846-a9e4-11ec-267d-d9895df88648
md"""
# Day 7: The Treachery of Whales #
## Problem 1 ##

A giant [whale](https://en.wikipedia.org/wiki/Sperm_whale) has decided your submarine is its next meal, and it's much faster than you are. There's nowhere to run!

Suddenly, a swarm of crabs (each in its own tiny submarine - it's too deep for them otherwise) zooms in to rescue you! They seem to be preparing to blast a hole in the ocean floor; sensors indicate a **massive underground cave system** just beyond where they're aiming!

The crab submarines all need to be aligned before they'll have enough power to blast a large enough hole for your submarine to get through. However, it doesn't look like they'll be aligned before the whale catches you! Maybe you can help?

There's one major catch - crab submarines can only move horizontally.

You quickly make a list of the **horizontal position of each crab** (your puzzle input). Crab submarines have limited fuel, so you need to find a way to make all of their horizontal positions match while requiring them to spend as little fuel as possible.

For example, consider the following horizontal positions:

16,1,2,0,4,2,7,1,2,14

This means there's a crab with horizontal position 16, a crab with horizontal position 1, and so on.

Each change of 1 step in horizontal position of a single crab costs 1 fuel. You could choose any horizontal position to align them all on, but the one that costs the least fuel is horizontal position 2:

    Move from 16 to 2: 14 fuel
    Move from 1 to 2: 1 fuel
    Move from 2 to 2: 0 fuel
    Move from 0 to 2: 2 fuel
    Move from 4 to 2: 2 fuel
    Move from 2 to 2: 0 fuel
    Move from 7 to 2: 5 fuel
    Move from 1 to 2: 1 fuel
    Move from 2 to 2: 0 fuel
    Move from 14 to 2: 12 fuel

This costs a total of **37** fuel. This is the cheapest possible outcome; more expensive outcomes include aligning at position 1 (41 fuel), position 3 (39 fuel), or position 10 (71 fuel).

Determine the horizontal position that the crabs can align to using the least fuel possible. **How much fuel must they spend to align to that position.**
"""

# ╔═╡ 7e411678-c457-4cf6-8389-889a7895cf11
problem_data = readline("data/crab_positions.dat");

# ╔═╡ ae2abc67-dd31-49b6-ba36-88137d715f88
initial_positions = [parse(Int, i) for i in split(problem_data, ",")];
# initial_positions = [16,1,2,0,4,2,7,1,2,14];

# ╔═╡ c9ba6048-53cd-4163-8e5f-fcd653d156ed
function calc_lowest_moves(crab_positions)
	allignment_pos = 0
	mean_dist = maximum(crab_positions) * size(crab_positions,1)
	println("Initial_max = ", mean_dist)
	for x in 1:maximum(crab_positions)
		current_dist = 0
		for y in crab_positions
			current_dist += abs(y - x)
			if current_dist > mean_dist
				break
			end
		end
		if current_dist < mean_dist
				mean_dist = current_dist
				allignment_pos = x
		end
	end
	return (mean_dist, allignment_pos)
end;	

# ╔═╡ 6cd7431f-5484-48f9-bdb2-d490c04e3d5e
min_fuel, allignment_pos = calc_lowest_moves(initial_positions);

# ╔═╡ e932dccf-b371-49fd-b427-6de0d8add852
md"""Crabs must expend **$(min_fuel)** units of fuel when alligned on position **$(allignment_pos)**"""

# ╔═╡ d5ecb8dc-fbc6-4fd3-94c0-508b5d985174
md"""
## Problem Two ##

The crabs don't seem interested in your proposed solution. Perhaps you misunderstand crab engineering?

As it turns out, crab submarine engines don't burn fuel at a constant rate. Instead, each change of 1 step in horizontal position costs 1 more unit of fuel than the last: the first step costs 1, the second step costs 2, the third step costs 3, and so on.

As each crab moves, moving further becomes more expensive. This changes the best horizontal position to align them all on; in the example above, this becomes 5:

    Move from 16 to 5: 66 fuel
    Move from 1 to 5: 10 fuel
    Move from 2 to 5: 6 fuel
    Move from 0 to 5: 15 fuel
    Move from 4 to 5: 1 fuel
    Move from 2 to 5: 6 fuel
    Move from 7 to 5: 3 fuel
    Move from 1 to 5: 10 fuel
    Move from 2 to 5: 6 fuel
    Move from 14 to 5: 45 fuel

This costs a total of **168** fuel. This is the new cheapest possible outcome; the old alignment position (2) now costs 206 fuel instead.

Determine the horizontal position that the crabs can align to using the least fuel possible so they can make you an escape route! **How much fuel must they spend to align to that position?**
"""

# ╔═╡ 65442cb4-0405-4330-b2d2-9ec6d6bab6cd
md"""
## Solution 2 ##
---

In this case the fuel consumed is the arithmetic sum of the distance moved; `` s_n = \frac{n(a_1 + a_n)}{2}``. In this case as we are taking the arithmetic sum of a distance moved (``d``) ``a_1 = 1`` and ``a_n = d`` .  The formula then reduces to:

```math
\displaystyle \frac{d^2 + d}{2}
```
"""

# ╔═╡ a654f8fc-574c-4729-a4a7-1e16bd1e139b
function calc_new_lowest_moves(crab_positions)
	allignment_pos = 0
	mean_dist = (maximum(crab_positions) * size(crab_positions,1))^2
	for x in 1:maximum(crab_positions)
		current_dist = 0
		for y in crab_positions
			move = abs(y - x)
			current_dist += Int((move^2 + move) /2)
			if current_dist > mean_dist
				break
			end
		end
		if current_dist < mean_dist
			mean_dist = current_dist
			allignment_pos = x
		end
	end
	return (mean_dist, allignment_pos)
end;	

# ╔═╡ 7eb8b24c-d61f-427a-b223-ab50bae8c615
new_min_fuel, new_allignment_pos = calc_new_lowest_moves(initial_positions);

# ╔═╡ d6920d63-2aa6-4072-a8f9-ac89be5ec1d8
md"""Crabs must now expend **$(new_min_fuel)** units of fuel when alligned on position **$(new_allignment_pos)**"""

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
# ╟─b81df846-a9e4-11ec-267d-d9895df88648
# ╠═7e411678-c457-4cf6-8389-889a7895cf11
# ╠═ae2abc67-dd31-49b6-ba36-88137d715f88
# ╠═c9ba6048-53cd-4163-8e5f-fcd653d156ed
# ╠═6cd7431f-5484-48f9-bdb2-d490c04e3d5e
# ╠═e932dccf-b371-49fd-b427-6de0d8add852
# ╟─d5ecb8dc-fbc6-4fd3-94c0-508b5d985174
# ╟─65442cb4-0405-4330-b2d2-9ec6d6bab6cd
# ╠═a654f8fc-574c-4729-a4a7-1e16bd1e139b
# ╠═7eb8b24c-d61f-427a-b223-ab50bae8c615
# ╟─d6920d63-2aa6-4072-a8f9-ac89be5ec1d8
# ╟─00000000-0000-0000-0000-000000000001
# ╟─00000000-0000-0000-0000-000000000002
