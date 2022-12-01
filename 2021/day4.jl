### A Pluto.jl notebook ###
# v0.18.1

using Markdown
using InteractiveUtils

# ╔═╡ 93339683-69bb-432a-934e-eb0dffdef9e9
using PlutoUI

# ╔═╡ 43252eee-9e43-11ec-3503-41f002536f67
md"""
# Day 4: Giant Squid #

## Problem 1 ##

You're already almost 1.5km (almost a mile) below the surface of the ocean, already so deep that you can't see any sunlight. What you **can** see, however, is a giant squid that has attached itself to the outside of your submarine.

Maybe it wants to play [bingo](https://en.wikipedia.org/wiki/Bingo_(American_version))?

Bingo is played on a set of boards each consisting of a 5x5 grid of numbers. Numbers are chosen at random, and the chosen number is **marked** on all boards on which it appears. (Numbers may not appear on all boards.) If all numbers in any row or any column of a board are marked, that board **wins**. (Diagonals don't count.)

The submarine has a **bingo subsystem** to help passengers (currently, you and the giant squid) pass the time. It automatically generates a random order in which to draw numbers and a random set of boards (your puzzle input). For example:
```
7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1
```

|    |    |     |    |    |   |   |   |   |   |   |   |   |   |   |   |   |
|----|----|-----|----|----|---|---|---|---|---|---|---|---|---|---|---|---|
| 22 | 13 | 17  | **11** | 0  |   | 3 |15 |  0|  2| 22|   | 14| 21| 17| 24|  **4**|
| 8  | 2  | 23  | **4**  | 24 |   | **9** |18 | 13| 17| **5**|   | 10| 16| 15|  **9**| 19|
| 21 | **9**  | 14  | 16 | **7**|   | 19|  8|  **7**| 25| 23|   | 18|  8| 23| 26| 20|
| 6  | 10 | 3   | 18 | **5**|   | 20| **11**| 10| 24|  **4**|   | 22| **11**| 13|  6|  **5**|
| 1  | 12 | 20  | 15 | 19 |   |  14| 21| 16|  12|  6|   |  2|  0| 12|  3| **7**|

After the first five numbers are drawn (7, 4, 9, 5, and 11), there are no winners, but the boards are marked as follows (shown here adjacent to each other to save space):

After the next six numbers are drawn (17, 23, 2, 0, 14, and 21), there are still no winners:

|    |    |     |    |    |   |   |   |   |   |   |   |   |   |   |   |   |
|----|----|-----|----|----|---|---|---|---|---|---|---|---|---|---|---|---|
| 22 | 13 | **17**  | **11** | 0  |   | 3 |15 | **0**| **2**| 22|   | **14**| **21**| **17**| 24|  **4**|
| 8  | 2  | **23**  | **4**  | 24 |   | **9** |18 | 13| 17| **5**|   | 10| 16| 15|  **9**| 19|
| **21** | **9**  | **14**  | 16 | **7**|   | 19|  8|  **7**| 25| **23**|   | 18|  8| **23**| 26| 20|
| 6  | 10 | 3   | 18 | **5**|   | 20| **11**| 10| 24|  **4**|   | 22| **11**| 13|  6|  **5**|
| 1  | 12 | 20  | 15 | 19 |   |  2|  0| 12|  3|  7|   |  2|  0| 12|  3| **7**|

Finally, 24 is drawn:

|    |    |     |    |    |   |   |   |   |   |   |   |   |   |   |   |   |
|----|----|-----|----|----|---|---|---|---|---|---|---|---|---|---|---|---|
| 22 | 13 | **17**  | **11** | 0  |   | 3 |15 | **0**| **2**| 22|   | **14**| **21**| **17**| **24**|  **4**|
| 8  | 2  | **23**  | **4**  | **24** |   | **9** |18 | 13| 17| **5**|   | 10| 16| 15|  **9**| 19|
| **21** | **9**  | **14**  | 16 | **7**|   | 19|  8|  **7**| 25| **23**|   | 18|  8| **23**| 26| 20|
| 6  | 10 | 3   | 18 | **5**|   | 20| **11**| 10| **24**|  **4**|   | 22| **11**| 13|  6|  **5**|
| 1  | 12 | 20  | 15 | 19 |   |  2|  0| 12|  3|  7|   |  2|  0| 12|  3| **7**|

At this point, the third board wins because it has at least one complete row or column of marked numbers (in this case, the entire top row is marked: **14 21 17 24 4**).

The **score** of the winning board can now be calculated. Start by finding the sum of **all unmarked numbers** on that board; in this case, the sum is 188. Then, multiply that sum by **the number that was just called** when the board won, 24, to get the final score, 188 * 24 = **4512**.

To guarantee victory against the giant squid, figure out which board will win first. **What will your final score be if you choose that board**?
"""

# ╔═╡ da99f331-8cae-46d7-8692-6d26b3233b08
md"""### Solution 1 ###
---

Set up the packages required
"""

# ╔═╡ ad0de5fa-e9c7-4bdf-90be-27870e406c76
lines = readlines("data/bingo.dat")

# ╔═╡ fc97718c-147f-432b-8ee9-965335a1e283
BOARD_SIZE = 5;

# ╔═╡ 3aa94063-0837-47b7-93c1-d75e0d06aa22
function create_boards(input)
	lines_index  = 1
	boards = Matrix{Int}[]

	while true
		if lines_index > size(input)[1]
			break
		end

		board = zeros(Int, BOARD_SIZE, BOARD_SIZE)
		for (index, row) in enumerate(input[lines_index:lines_index + BOARD_SIZE - 1])
			board_row = [parse(Int, i) for i in filter(x -> x != "", split(row, " "))]
			board[index,:] = board_row
		end

		push!(boards, board)

		lines_index += BOARD_SIZE + 1
	end

	return boards
end;

# ╔═╡ b08d1695-27f8-4d15-aedd-e1ce60fb80e1
bingo_calls =[parse(Int, i) for i in split(lines[1], ",")];

# ╔═╡ 5314f0d5-cfa2-4827-982e-aa8bc3ced900
boards = create_boards(lines[3:end]);

# ╔═╡ af791444-a22d-4312-842a-e774146a1500
function check_if_winner(board, numbers)
	"""
	Given a set of numbers check if the a line or column of the 
	board is a subset of the numbers.  If so return true, if not return false
	"""
	for σ ∈ 1 : BOARD_SIZE
		if issubset(board[σ,:], numbers) || issubset(board[:, σ], numbers)
			return true
		end
	end

	return false
end;

# ╔═╡ 0050ef6f-9596-4381-b540-601cebda7153
function get_1st_winning_board(boards, numbers)
	"""
	Starting with the first five numbers step through the boards checking if a board
	has won. If not keep adding the next number to check until a winning board is
	found.
	return the winning board and the winning number.
	"""

	for i ∈ 5: size(numbers)[1]
		for board ∈ boards
			if check_if_winner(board, numbers[1:i])
				return (board, numbers[1:i])
			end
		end
	end

	return (nothing, nothing)
end;

# ╔═╡ 92fdd060-0db4-4ba6-bc6b-449ec2956bf7
winning_board, drawn_numbers = get_1st_winning_board(boards, bingo_calls);

# ╔═╡ 873aab0b-80ff-4b77-beb1-e9754f9a4c17
function calculate_points(board, numbers)
	uncalled_sum = 0
	for row ∈ board
		for element ∈ row
			if !(element ∈ numbers)
				uncalled_sum += element
			end
		end
	end
	return uncalled_sum * last(numbers)
end;

# ╔═╡ 2fe312ca-c3fd-471c-9e58-7c437a17108c
md"""The final score for the winning board is: $[calculate_points(winning_board, drawn_numbers)] points"""

# ╔═╡ b172a4c2-57e5-49ec-be89-224f9165a450
md"""
## Problem Two ##

On the other hand, it might be wise to try a different strategy: let the giant squid win.

You aren't sure how many bingo boards a giant squid could play at once, so rather than waste time counting its arms, the safe thing to do is to **figure out which board will win last** and choose that one. That way, no matter which boards it picks, it will win for sure.

In the above example, the second board is the last to win, which happens after 13 is eventually called and its middle column is completely marked. If you were to keep playing until this point, the second board would have a sum of unmarked numbers equal to 148 for a final score of 148 * 13 = **1924**.

Figure out which board will win last. **Once it wins, what would its final score be?**
"""

# ╔═╡ 2e06acfc-ac98-41a9-878e-172302b7e45c
md"""
### Solution 2 ###
---

In this case we start at with all the calls as that should mean that every board has won!. Reduce the call list by one every time and check if there is a board that hasn't won - this must be the last winning board.  The final number called will be the one next after the current call list.
"""

# ╔═╡ d38d3a2a-149c-496e-92a9-c685bf0356b7
function get_last_winning_card(boards, numbers)
	for i ∈ size(numbers)[1]:-1:5
		for board in boards
			found = check_if_winner(board, numbers[1:i])
			if !found
				return(board, numbers[1:i+1])
			end
		end
	end
	return (nothing, nothing)
end;

# ╔═╡ f5f40bc2-9372-4b3c-bba1-10c184ceed0b
board, calls = get_last_winning_card(boards, bingo_calls);

# ╔═╡ 1a2751f2-a356-4886-8374-1428f8acd669
md"""The final score for the winning board is: $[calculate_points(board, calls)] points"""

# ╔═╡ 00000000-0000-0000-0000-000000000001
PLUTO_PROJECT_TOML_CONTENTS = """
[deps]
PlutoUI = "7f904dfe-b85e-4ff6-b463-dae2292396a8"

[compat]
PlutoUI = "~0.7.36"
"""

# ╔═╡ 00000000-0000-0000-0000-000000000002
PLUTO_MANIFEST_TOML_CONTENTS = """
# This file is machine-generated - editing it directly is not advised

julia_version = "1.7.2"
manifest_format = "2.0"

[[deps.AbstractPlutoDingetjes]]
deps = ["Pkg"]
git-tree-sha1 = "8eaf9f1b4921132a4cff3f36a1d9ba923b14a481"
uuid = "6e696c72-6542-2067-7265-42206c756150"
version = "1.1.4"

[[deps.ArgTools]]
uuid = "0dad84c5-d112-42e6-8d28-ef12dabb789f"

[[deps.Artifacts]]
uuid = "56f22d72-fd6d-98f1-02f0-08ddc0907c33"

[[deps.Base64]]
uuid = "2a0f44e3-6c83-55bd-87e4-b1978d98bd5f"

[[deps.ColorTypes]]
deps = ["FixedPointNumbers", "Random"]
git-tree-sha1 = "024fe24d83e4a5bf5fc80501a314ce0d1aa35597"
uuid = "3da002f7-5984-5a60-b8a6-cbb66c0b333f"
version = "0.11.0"

[[deps.CompilerSupportLibraries_jll]]
deps = ["Artifacts", "Libdl"]
uuid = "e66e0078-7015-5450-92f7-15fbd957f2ae"

[[deps.Dates]]
deps = ["Printf"]
uuid = "ade2ca70-3891-5945-98fb-dc099432e06a"

[[deps.Downloads]]
deps = ["ArgTools", "LibCURL", "NetworkOptions"]
uuid = "f43a241f-c20a-4ad4-852c-f6b1247861c6"

[[deps.FixedPointNumbers]]
deps = ["Statistics"]
git-tree-sha1 = "335bfdceacc84c5cdf16aadc768aa5ddfc5383cc"
uuid = "53c48c17-4a7d-5ca2-90c5-79b7896eea93"
version = "0.8.4"

[[deps.Hyperscript]]
deps = ["Test"]
git-tree-sha1 = "8d511d5b81240fc8e6802386302675bdf47737b9"
uuid = "47d2ed2b-36de-50cf-bf87-49c2cf4b8b91"
version = "0.0.4"

[[deps.HypertextLiteral]]
git-tree-sha1 = "2b078b5a615c6c0396c77810d92ee8c6f470d238"
uuid = "ac1192a8-f4b3-4bfe-ba22-af5b92cd3ab2"
version = "0.9.3"

[[deps.IOCapture]]
deps = ["Logging", "Random"]
git-tree-sha1 = "f7be53659ab06ddc986428d3a9dcc95f6fa6705a"
uuid = "b5f81e59-6552-4d32-b1f0-c071b021bf89"
version = "0.2.2"

[[deps.InteractiveUtils]]
deps = ["Markdown"]
uuid = "b77e0a4c-d291-57a0-90e8-8db25a27a240"

[[deps.JSON]]
deps = ["Dates", "Mmap", "Parsers", "Unicode"]
git-tree-sha1 = "3c837543ddb02250ef42f4738347454f95079d4e"
uuid = "682c06a0-de6a-54ab-a142-c8b1cf79cde6"
version = "0.21.3"

[[deps.LibCURL]]
deps = ["LibCURL_jll", "MozillaCACerts_jll"]
uuid = "b27032c2-a3e7-50c8-80cd-2d36dbcbfd21"

[[deps.LibCURL_jll]]
deps = ["Artifacts", "LibSSH2_jll", "Libdl", "MbedTLS_jll", "Zlib_jll", "nghttp2_jll"]
uuid = "deac9b47-8bc7-5906-a0fe-35ac56dc84c0"

[[deps.LibGit2]]
deps = ["Base64", "NetworkOptions", "Printf", "SHA"]
uuid = "76f85450-5226-5b5a-8eaa-529ad045b433"

[[deps.LibSSH2_jll]]
deps = ["Artifacts", "Libdl", "MbedTLS_jll"]
uuid = "29816b5a-b9ab-546f-933c-edad1886dfa8"

[[deps.Libdl]]
uuid = "8f399da3-3557-5675-b5ff-fb832c97cbdb"

[[deps.LinearAlgebra]]
deps = ["Libdl", "libblastrampoline_jll"]
uuid = "37e2e46d-f89d-539d-b4ee-838fcccc9c8e"

[[deps.Logging]]
uuid = "56ddb016-857b-54e1-b83d-db4d58db5568"

[[deps.Markdown]]
deps = ["Base64"]
uuid = "d6f4376e-aef5-505a-96c1-9c027394607a"

[[deps.MbedTLS_jll]]
deps = ["Artifacts", "Libdl"]
uuid = "c8ffd9c3-330d-5841-b78e-0817d7145fa1"

[[deps.Mmap]]
uuid = "a63ad114-7e13-5084-954f-fe012c677804"

[[deps.MozillaCACerts_jll]]
uuid = "14a3606d-f60d-562e-9121-12d972cd8159"

[[deps.NetworkOptions]]
uuid = "ca575930-c2e3-43a9-ace4-1e988b2c1908"

[[deps.OpenBLAS_jll]]
deps = ["Artifacts", "CompilerSupportLibraries_jll", "Libdl"]
uuid = "4536629a-c528-5b80-bd46-f80d51c5b363"

[[deps.Parsers]]
deps = ["Dates"]
git-tree-sha1 = "85b5da0fa43588c75bb1ff986493443f821c70b7"
uuid = "69de0a69-1ddd-5017-9359-2bf0b02dc9f0"
version = "2.2.3"

[[deps.Pkg]]
deps = ["Artifacts", "Dates", "Downloads", "LibGit2", "Libdl", "Logging", "Markdown", "Printf", "REPL", "Random", "SHA", "Serialization", "TOML", "Tar", "UUIDs", "p7zip_jll"]
uuid = "44cfe95a-1eb2-52ea-b672-e2afdf69b78f"

[[deps.PlutoUI]]
deps = ["AbstractPlutoDingetjes", "Base64", "ColorTypes", "Dates", "Hyperscript", "HypertextLiteral", "IOCapture", "InteractiveUtils", "JSON", "Logging", "Markdown", "Random", "Reexport", "UUIDs"]
git-tree-sha1 = "2c87c85e397b7ffed5ffec054f532d4edd05d901"
uuid = "7f904dfe-b85e-4ff6-b463-dae2292396a8"
version = "0.7.36"

[[deps.Printf]]
deps = ["Unicode"]
uuid = "de0858da-6303-5e67-8744-51eddeeeb8d7"

[[deps.REPL]]
deps = ["InteractiveUtils", "Markdown", "Sockets", "Unicode"]
uuid = "3fa0cd96-eef1-5676-8a61-b3b8758bbffb"

[[deps.Random]]
deps = ["SHA", "Serialization"]
uuid = "9a3f8284-a2c9-5f02-9a11-845980a1fd5c"

[[deps.Reexport]]
git-tree-sha1 = "45e428421666073eab6f2da5c9d310d99bb12f9b"
uuid = "189a3867-3050-52da-a836-e630ba90ab69"
version = "1.2.2"

[[deps.SHA]]
uuid = "ea8e919c-243c-51af-8825-aaa63cd721ce"

[[deps.Serialization]]
uuid = "9e88b42a-f829-5b0c-bbe9-9e923198166b"

[[deps.Sockets]]
uuid = "6462fe0b-24de-5631-8697-dd941f90decc"

[[deps.SparseArrays]]
deps = ["LinearAlgebra", "Random"]
uuid = "2f01184e-e22b-5df5-ae63-d93ebab69eaf"

[[deps.Statistics]]
deps = ["LinearAlgebra", "SparseArrays"]
uuid = "10745b16-79ce-11e8-11f9-7d13ad32a3b2"

[[deps.TOML]]
deps = ["Dates"]
uuid = "fa267f1f-6049-4f14-aa54-33bafae1ed76"

[[deps.Tar]]
deps = ["ArgTools", "SHA"]
uuid = "a4e569a6-e804-4fa4-b0f3-eef7a1d5b13e"

[[deps.Test]]
deps = ["InteractiveUtils", "Logging", "Random", "Serialization"]
uuid = "8dfed614-e22c-5e08-85e1-65c5234f0b40"

[[deps.UUIDs]]
deps = ["Random", "SHA"]
uuid = "cf7118a7-6976-5b1a-9a39-7adc72f591a4"

[[deps.Unicode]]
uuid = "4ec0a83e-493e-50e2-b9ac-8f72acf5a8f5"

[[deps.Zlib_jll]]
deps = ["Libdl"]
uuid = "83775a58-1f1d-513f-b197-d71354ab007a"

[[deps.libblastrampoline_jll]]
deps = ["Artifacts", "Libdl", "OpenBLAS_jll"]
uuid = "8e850b90-86db-534c-a0d3-1478176c7d93"

[[deps.nghttp2_jll]]
deps = ["Artifacts", "Libdl"]
uuid = "8e850ede-7688-5339-a07c-302acd2aaf8d"

[[deps.p7zip_jll]]
deps = ["Artifacts", "Libdl"]
uuid = "3f19e933-33d8-53b3-aaab-bd5110c3b7a0"
"""

# ╔═╡ Cell order:
# ╟─43252eee-9e43-11ec-3503-41f002536f67
# ╟─da99f331-8cae-46d7-8692-6d26b3233b08
# ╠═93339683-69bb-432a-934e-eb0dffdef9e9
# ╠═ad0de5fa-e9c7-4bdf-90be-27870e406c76
# ╠═fc97718c-147f-432b-8ee9-965335a1e283
# ╠═3aa94063-0837-47b7-93c1-d75e0d06aa22
# ╠═b08d1695-27f8-4d15-aedd-e1ce60fb80e1
# ╠═5314f0d5-cfa2-4827-982e-aa8bc3ced900
# ╠═af791444-a22d-4312-842a-e774146a1500
# ╠═0050ef6f-9596-4381-b540-601cebda7153
# ╠═92fdd060-0db4-4ba6-bc6b-449ec2956bf7
# ╠═873aab0b-80ff-4b77-beb1-e9754f9a4c17
# ╠═2fe312ca-c3fd-471c-9e58-7c437a17108c
# ╟─b172a4c2-57e5-49ec-be89-224f9165a450
# ╟─2e06acfc-ac98-41a9-878e-172302b7e45c
# ╠═d38d3a2a-149c-496e-92a9-c685bf0356b7
# ╠═f5f40bc2-9372-4b3c-bba1-10c184ceed0b
# ╟─1a2751f2-a356-4886-8374-1428f8acd669
# ╟─00000000-0000-0000-0000-000000000001
# ╟─00000000-0000-0000-0000-000000000002
