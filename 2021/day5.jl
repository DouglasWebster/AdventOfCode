### A Pluto.jl notebook ###
# v0.17.7

using Markdown
using InteractiveUtils

# ╔═╡ 2a495dd4-a6db-11ec-0f26-1d2f500024ee
md"""
# Day 5: Hydrothermal Venture #
## Problem 1 ##

You come across a field of [hydrothermal vents](https://en.wikipedia.org/wiki/Hydrothermal_vent) on the ocean floor! These vents constantly produce large, opaque clouds, so it would be best to avoid them if possible.

They tend to form in lines; the submarine helpfully produces a list of nearby lines of vents (your puzzle input) for you to review. For example:
```
0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2
```
Each line of vents is given as a line segment in the format x1,y1 -> x2,y2 where x1,y1 are the coordinates of one end the line segment and x2,y2 are the coordinates of the other end. These line segments include the points at both ends. In other words:

    An entry like 1,1 -> 1,3 covers points 1,1, 1,2, and 1,3.
    An entry like 9,7 -> 7,7 covers points 9,7, 8,7, and 7,7.

For now, **only consider horizontal and vertical lines**: lines where either x1 = x2 or y1 = y2.

So, the horizontal and vertical lines from the above list would produce the following diagram:
```
.......1..
..1....1..
..1....1..
.......1..
.112111211
..........
..........
..........
..........
222111....
```
In this diagram, the top left corner is 0,0 and the bottom right corner is 9,9. Each position is shown as **the number of lines which cover that point** or **.** if no line covers that point. The top-left pair of 1s, for example, comes from 2,2 -> 2,1; the very bottom row is formed by the overlapping lines 0,9 -> 5,9 and 0,9 -> 2,9.

To avoid the most dangerous areas, you need to determine **the number of points where at least two lines overlap**. In the above example, this is anywhere in the diagram with a 2 or larger - a total of **5** points.

Consider only horizontal and vertical lines. **At how many points do at least two lines overlap?**
"""

# ╔═╡ fdb11e78-8c2c-4291-99e7-df924d85bc61
md"""
## Solution 1 ##
---
Set up the packages required
"""

# ╔═╡ ca58dbde-04ef-475b-b1cc-fe30af761d6d
line_coords = readlines("data/vents.dat")

# ╔═╡ 278b5f7f-5ead-48e8-872a-06e4f3b48830
md"""
Extract the co-ordinates and convert them to integers.
"""

# ╔═╡ c956d7c4-72fe-49cb-a9fa-d08a415f928f
"""
	extract_coords(string)

Given a string of the form `x1,y1 -> x2,y2` return an array of the form 
`[[x1, y1],[x2, y2]]`
"""
function extract_coords(coord_line)
	coords = split(coord_line, " -> ")
	coord_1 = split(coords[1], ",")
	coord_2 = split(coords[2], ",")
	x1 = parse(Int, coord_1[1])
	y1 = parse(Int, coord_1[2])
	x2 = parse(Int, coord_2[1])
	y2 = parse(Int, coord_2[2])
	coord_array = [[x1, y1],[x2,y2]]
	return coord_array
	
end;

# ╔═╡ 5432ed31-74d8-455d-a4e3-7cc084aecc1c


# ╔═╡ 49394606-17ca-4ebc-8a58-e2ca1aa9a4ee
vent_coords = [];

# ╔═╡ c96a23af-73dd-4d65-99f6-032df23ce1d8
for n ∈ line_coords
	converted_coords = extract_coords(n)
	push!(vent_coords, converted_coords)
end

# ╔═╡ 75f74675-c90a-44f2-b723-241f7ec313b0
md"""
Create two arrays, one to hold the orthogonal data and on for the diagonal data.  The arrays are initially all zeros and each line in the co-ordinate array increase the corresponding line points  in the relevant array depending on whether the line is orthogonal or diagonal.
"""

# ╔═╡ ee8044d2-35c7-4b57-a70c-363e66b89e00
function create_entries(coords)
	orthogonal_vents = zeros(Int, (1000, 1000))
	diagonal_vents = zeros(Int, (1000, 1000))
	for coord in coords
		# careful here, co-ordinate index's are zero based but julia is 1 based!
		x1 = coord[1][1] + 1
		y1 = coord[1][2] + 1
		x2 = coord[2][1] + 1
		y2 = coord[2][2] + 1
	
		
		if x1 == x2
			y1 < y2 ? step =1 : step = -1
			for n ∈ y1:step:y2
				orthogonal_vents[n,x1] += 1
			end
		elseif y1 == y2
			x1 < x2 ? step = 1 : step = -1
			for n ∈ x1:step:x2
				orthogonal_vents[y1, n] += 1
			end
		elseif abs(x1-x2) == abs(y1-y2)
			x1 < x2 ? stepX = 1 : stepX = -1
			x_range = collect(Int, x1:stepX:x2)
			y1 < y2 ? stepY =1 : stepY = -1
			y_range = collect(Int, y1:stepY:y2)
			for (x,y) in zip(x_range, y_range)
				diagonal_vents[y,x] += 1
			end
		end
    end
	return (orthogonal_vents, diagonal_vents)
end;

# ╔═╡ daf1cb36-b952-47a4-80e2-dca8e1b32791
(orth_vent_matrix, diag_vent_matrix) = create_entries(vent_coords);

# ╔═╡ 4b4eb2a6-3fde-450e-88d7-084bd2ed9777
md"""
For Part 1 we just count the number of entries in the orthogonal matrix where the count is greater than 1.
"""

# ╔═╡ 35127557-ba41-4479-a56a-d2cf019c49e1
count(i -> (i > 1), orth_vent_matrix)

# ╔═╡ df4769fa-5ed4-481a-a8f2-0fd4aed6856e
md"""
## Part Two ##

Unfortunately, considering only horizontal and vertical lines doesn't give you the full picture; you need to also consider **diagonal lines**.

Because of the limits of the hydrothermal vent mapping system, the lines in your list will only ever be horizontal, vertical, or a diagonal line at exactly 45 degrees. In other words:

    An entry like 1,1 -> 3,3 covers points 1,1, 2,2, and 3,3.
    An entry like 9,7 -> 7,9 covers points 9,7, 8,8, and 7,9.

Considering all lines from the above example would now produce the following diagram:
```
1.1....11.
.111...2..
..2.1.111.
...1.2.2..
.112313211
...1.2....
..1...1...
.1.....1..
1.......1.
222111....
```
You still need to determine **the number of points where at least two lines overlap**. In the above example, this is still anywhere in the diagram with a 2 or larger - now a total of **12** points.

Consider all of the lines. **At how many points do at least two lines overlap?**
"""

# ╔═╡ c3942e7e-5f48-4ea7-bf81-d95fedbaef56
md"""
### Solution 2 ###
---
This becomes a relatively simple solution now; just add the orthogonal and diagonal matrices together and then count the number of entries greater than 1.
"""

# ╔═╡ ba5c74de-68b2-40de-ab6b-132098616582
full_matrix = diag_vent_matrix + orth_vent_matrix;

# ╔═╡ b89298ba-361a-4a48-b692-ff05597ef8d5
count(i -> (i > 1), full_matrix)

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
# ╟─2a495dd4-a6db-11ec-0f26-1d2f500024ee
# ╟─fdb11e78-8c2c-4291-99e7-df924d85bc61
# ╠═ca58dbde-04ef-475b-b1cc-fe30af761d6d
# ╟─278b5f7f-5ead-48e8-872a-06e4f3b48830
# ╠═c956d7c4-72fe-49cb-a9fa-d08a415f928f
# ╠═5432ed31-74d8-455d-a4e3-7cc084aecc1c
# ╠═49394606-17ca-4ebc-8a58-e2ca1aa9a4ee
# ╠═c96a23af-73dd-4d65-99f6-032df23ce1d8
# ╟─75f74675-c90a-44f2-b723-241f7ec313b0
# ╠═ee8044d2-35c7-4b57-a70c-363e66b89e00
# ╠═daf1cb36-b952-47a4-80e2-dca8e1b32791
# ╟─4b4eb2a6-3fde-450e-88d7-084bd2ed9777
# ╠═35127557-ba41-4479-a56a-d2cf019c49e1
# ╟─df4769fa-5ed4-481a-a8f2-0fd4aed6856e
# ╟─c3942e7e-5f48-4ea7-bf81-d95fedbaef56
# ╠═ba5c74de-68b2-40de-ab6b-132098616582
# ╠═b89298ba-361a-4a48-b692-ff05597ef8d5
# ╟─00000000-0000-0000-0000-000000000001
# ╟─00000000-0000-0000-0000-000000000002
