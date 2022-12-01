### A Pluto.jl notebook ###
# v0.18.1

using Markdown
using InteractiveUtils

# ╔═╡ 5f6ebd8b-cdb7-4166-8191-2213375d71cc
using DelimitedFiles, PlutoUI

# ╔═╡ f5a34296-9c99-11ec-0b8f-b9eb46485323
md"""
# Day 3: Binary Diagnostic #
## Problem 1 ##
The submarine has been making some odd creaking noises, so you ask it to produce a diagnostic report just in case.

The diagnostic report (your puzzle input) consists of a list of binary numbers which, when decoded properly, can tell you many useful things about the conditions of the submarine. The first parameter to check is the **power consumption.**

You need to use the binary numbers in the diagnostic report to generate two new binary numbers (called the **gamma rate** and the **epsilon rate**). The power consumption can then be found by multiplying the gamma rate by the epsilon rate.

Each bit in the gamma rate can be determined by finding the **most common bit in the corresponding position** of all numbers in the diagnostic report. For example, given the following diagnostic report:
```
00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010
```
Considering only the first bit of each number, there are five 0 bits and seven 1 bits. Since the most common bit is 1, the first bit of the gamma rate is 1.

The most common second bit of the numbers in the diagnostic report is 0, so the second bit of the gamma rate is 0.

The most common value of the third, fourth, and fifth bits are 1, 1, and 0, respectively, and so the final three bits of the gamma rate are 110.

So, the gamma rate is the binary number 10110, or **22** in decimal.

The epsilon rate is calculated in a similar way; rather than use the most common bit, the least common bit from each position is used. So, the epsilon rate is 01001, or 9 in decimal. Multiplying the gamma rate (22) by the epsilon rate (9) produces the power consumption, **198**.

Use the binary numbers in your diagnostic report to calculate the gamma rate and epsilon rate, then multiply them together. **What is the power consumption of the submarine?** (Be sure to represent your answer in decimal, not binary.)
"""

# ╔═╡ a165976c-14ed-494a-a785-282e0ead585b
md"""### Solution 1 ###
---

Set up the packages required
"""

# ╔═╡ 02eed8e1-3c03-478f-bef9-d8fe55f5c001
diagnostics = readdlm("data/diagnostic.dat", String);

# ╔═╡ 374ce0cc-b13c-4bbb-8419-532c3e9b5d8f
begin
	gamma = ""
	epsilon = ""
	for x ∈ 1: length(diagnostics[1])
		is_gamma = 0
		for y ∈ diagnostics
			y[x] == '1' ? is_gamma += 1 : is_gamma -= 1
		end
		is_gamma ≥ 0 ? (gamma *= "1"; epsilon *= "0") : (gamma *= "0"; epsilon *= "1")
	end
	println(gamma, " ", epsilon)
end

# ╔═╡ 60189862-808e-4862-9e87-18841bf1ab1b
epsilon_rate = parse(Int, epsilon, base=2);

# ╔═╡ d4696ce1-9a7a-4b9e-bb9b-1a0d4b9035b4
gamma_rate = parse(Int, gamma, base=2);

# ╔═╡ 0237ac29-2954-4edd-b0cc-7ce0a772f557
md"""
```
Print("Power Consumption: ", gamma_rate * epsilon_rate)
```
"""

# ╔═╡ 4445c81b-89fd-4c8a-8cf9-cfe04a70a164
Print("Power Consumption: ", gamma_rate * epsilon_rate)

# ╔═╡ 70a50d7e-091e-46e0-9e62-f885ada71c5e
md"""
## Part Two ##

Next, you should verify the **life support rating**, which can be determined by multiplying the **oxygen generator** rating by the **CO2 scrubber rating**.

Both the oxygen generator rating and the CO2 scrubber rating are values that can be found in your diagnostic report - finding them is the tricky part. Both values are located using a similar process that involves filtering out values until only one remains. Before searching for either rating value, start with the full list of binary numbers from your diagnostic report and **consider just the first bit** of those numbers. Then:

- Keep only numbers selected by the **bit criteria** for the type of rating value for which you are searching. Discard numbers which do not match the bit criteria.
- If you only have one number left, stop; this is the rating value for which you are searching.
- Otherwise, repeat the process, considering the next bit to the right.

The **bit criteria** depends on which type of rating value you want to find:

- To find **oxygen generator rating**, determine the **most common** value (0 or 1) in the current bit position, and keep only numbers with that bit in that position. If 0 and 1 are equally common, keep values with a **1** in the position being considered.
- To find **CO2 scrubber rating**, determine the **least common** value (0 or 1) in the current bit position, and keep only numbers with that bit in that position. If 0 and 1 are equally common, keep values with a **0** in the position being considered.

For example, to determine the **oxygen generator** rating value using the same example diagnostic report from above:

- Start with all 12 numbers and consider only the first bit of each number. There are more 1 bits (7) than 0 bits (5) keep only the 7 numbers with a 1 in the first position: 11110, 10110, 10111, 10101, 11100, 10000, and 11001.
- Then, consider the second bit of the 7 remaining numbers: there are more 0 bits (4) than 1 bits (3) keep only the 4 numbers with a 0 in the second position: 10110, 10111, 10101, and 10000.
- In the third position, three of the four numbers have a 1 keep those three: 10110, 10111, and 10101.
- In the fourth position, two of the three numbers have a 1 keep those two: 10110 and 10111.
- In the fifth position, there are an equal number of 0 bits and 1 bits (one each). to find the oxygen generator rating, keep the number with a 1 in that position: 10111.
- As there is only one number left, stop; the oxygen generator rating is 10111, or 23 in decimal.

Then, to determine the **CO2 scrubber rating** value from the same example above:

- Start again with all 12 numbers and consider only the first bit of each number. There are fewer 0 bits (5) than 1 bits (7) keep only the 5 numbers with a 0 in the first position: 00100, 01111, 00111, 00010, and 01010.
- Then, consider the second bit of the 5 remaining numbers: there are fewer 1 bits (2) than 0 bits (3) keep only the 2 numbers with a 1 in the second position: 01111 and 01010.
- In the third position, there are an equal number of 0 bits and 1 bits (one each). to find the **CO2 scrubber rating**, keep the number with a 0 in that position: 01010.
- As there is only one number left, stop; the **CO2 scrubber rating** is 01010, or 10 in decimal.

Finally, to find the life support rating, multiply the oxygen generator rating (23) by the CO2 scrubber rating (10) to get **230**.

Use the binary numbers in your diagnostic report to calculate the oxygen generator rating and CO2 scrubber rating, then multiply them together. **What is the life support rating of the submarine?** (Be sure to represent your answer in decimal, not binary.)
"""

# ╔═╡ 30d38290-6576-4dcd-8fa6-2a23ea18880a
md"""
### Solution 2 ###
---
"""

# ╔═╡ 34cb16ba-fdc1-4a33-9e6c-81bed5932520
function get_bit_count(a, pos=1)
	msbs = map(x -> parse(Int8, x[pos]), a)
	no_of_ones = count(i -> (i == 1), msbs)
	no_of_zeros = length(a) - no_of_ones

	return no_of_ones, no_of_zeros
end;

# ╔═╡ 1a2627eb-a060-4a1b-9d66-9f0526e0d7d1
function reduce_O2(a)
	O₂Gen = map(x -> a[:,1], a)[1]
	no_of_bits = length(O₂Gen[1])
	x = 1
	while (length(O₂Gen) > 1)
		if x > no_of_bits
			break
		end
		one_bits, zero_bits = get_bit_count(O₂Gen, x)
		one_bits ≥ zero_bits ? keep_digit = '1' : keep_digit = '0'
		items_to_delete = []
		for y ∈ 1: length(O₂Gen)
			if O₂Gen[y][x] != keep_digit
				push!(items_to_delete, y)
			end
		end
		O₂Gen = deleteat!(O₂Gen, items_to_delete)
		x += 1
	end
	return O₂Gen
end;


# ╔═╡ 4929a48c-ecdb-43fe-a6bb-87b92637bc46
function reduce_CO2(a)
	CO₂Gen = map(x -> a[:,1], a)[1]
	no_of_bits = length(CO₂Gen[1])
	x = 1
	while (length(CO₂Gen) > 1)
		if x > no_of_bits
			break
		end
		one_bits, zero_bits = get_bit_count(CO₂Gen, x)
		zero_bits ≤ one_bits ? keep_digit = '0' : keep_digit = '1'
		items_to_delete = []
		for y ∈ 1: length(CO₂Gen)
			if CO₂Gen[y][x] != keep_digit
				push!(items_to_delete, y)
			end
		end
		CO₂Gen = deleteat!(CO₂Gen, items_to_delete)
		x += 1
	end
	return CO₂Gen
end;

# ╔═╡ 49602635-0d83-47d3-a193-296cd4c859f2
O₂Rating = parse(Int, reduce_O2(diagnostics)[1], base = 2);

# ╔═╡ 77366323-cd4f-4339-9311-123af147420c
CO₂Rating = parse(Int, reduce_CO2(diagnostics)[1], base = 2);

# ╔═╡ d8a6cb63-b4fc-45b0-9f84-1d9a1859692f
	Print("O2 Generator rating: ", O₂Rating, "\nCO2 Scrubber rating: ", CO₂Rating, "\n\nLife support rating: ", CO₂Rating * O₂Rating)


# ╔═╡ 519f0a24-a847-4108-aa5e-57788f4601ff
TableOfContents(title="Advent Of Code", depth=4)

# ╔═╡ 00000000-0000-0000-0000-000000000001
PLUTO_PROJECT_TOML_CONTENTS = """
[deps]
DelimitedFiles = "8bb1440f-4735-579b-a4ab-409b98df4dab"
PlutoUI = "7f904dfe-b85e-4ff6-b463-dae2292396a8"

[compat]
PlutoUI = "~0.7.35"
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

[[deps.DelimitedFiles]]
deps = ["Mmap"]
uuid = "8bb1440f-4735-579b-a4ab-409b98df4dab"

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
git-tree-sha1 = "13468f237353112a01b2d6b32f3d0f80219944aa"
uuid = "69de0a69-1ddd-5017-9359-2bf0b02dc9f0"
version = "2.2.2"

[[deps.Pkg]]
deps = ["Artifacts", "Dates", "Downloads", "LibGit2", "Libdl", "Logging", "Markdown", "Printf", "REPL", "Random", "SHA", "Serialization", "TOML", "Tar", "UUIDs", "p7zip_jll"]
uuid = "44cfe95a-1eb2-52ea-b672-e2afdf69b78f"

[[deps.PlutoUI]]
deps = ["AbstractPlutoDingetjes", "Base64", "ColorTypes", "Dates", "Hyperscript", "HypertextLiteral", "IOCapture", "InteractiveUtils", "JSON", "Logging", "Markdown", "Random", "Reexport", "UUIDs"]
git-tree-sha1 = "85bf3e4bd279e405f91489ce518dedb1e32119cb"
uuid = "7f904dfe-b85e-4ff6-b463-dae2292396a8"
version = "0.7.35"

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
# ╟─f5a34296-9c99-11ec-0b8f-b9eb46485323
# ╟─a165976c-14ed-494a-a785-282e0ead585b
# ╠═5f6ebd8b-cdb7-4166-8191-2213375d71cc
# ╠═02eed8e1-3c03-478f-bef9-d8fe55f5c001
# ╠═374ce0cc-b13c-4bbb-8419-532c3e9b5d8f
# ╠═60189862-808e-4862-9e87-18841bf1ab1b
# ╠═d4696ce1-9a7a-4b9e-bb9b-1a0d4b9035b4
# ╟─0237ac29-2954-4edd-b0cc-7ce0a772f557
# ╟─4445c81b-89fd-4c8a-8cf9-cfe04a70a164
# ╟─70a50d7e-091e-46e0-9e62-f885ada71c5e
# ╟─30d38290-6576-4dcd-8fa6-2a23ea18880a
# ╠═34cb16ba-fdc1-4a33-9e6c-81bed5932520
# ╠═1a2627eb-a060-4a1b-9d66-9f0526e0d7d1
# ╠═4929a48c-ecdb-43fe-a6bb-87b92637bc46
# ╠═49602635-0d83-47d3-a193-296cd4c859f2
# ╠═77366323-cd4f-4339-9311-123af147420c
# ╟─d8a6cb63-b4fc-45b0-9f84-1d9a1859692f
# ╟─519f0a24-a847-4108-aa5e-57788f4601ff
# ╟─00000000-0000-0000-0000-000000000001
# ╟─00000000-0000-0000-0000-000000000002
