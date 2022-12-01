line_coords = [
    "0,9 -> 5,9",
    "8,0 -> 0,8",
    "9,4 -> 3,4",
    "2,2 -> 2,1",
    "7,0 -> 7,4",
    "6,4 -> 2,0",
    "0,9 -> 2,9",
    "3,4 -> 1,4",
    "0,0 -> 8,8",
    "5,5 -> 8,2"
]

function extract_coords(coord_line)
    coords = split(coord_line, " -> ")
    coord_1 = split(coords[1], ",")
    coord_2 = split(coords[2], ",")
    x1 = parse(Int, coord_1[1])
    y1 = parse(Int, coord_1[2])
    x2 = parse(Int, coord_2[1])
    y2 = parse(Int, coord_2[2])
    coord_array = [[x1, y1], [x2, y2]]
    return coord_array

end


vent_coords = []


for n ∈ line_coords
    converted_coords = extract_coords(n)
    push!(vent_coords, converted_coords)
end


function create_orthogonal_lines(coords)
    vents = zeros(Int, (10, 10))
    for coord in coords
        # careful here, co-ordinate index's are zero based but julia is 1 based!
        x1 = coord[1][1] + 1
        y1 = coord[1][2] + 1
        x2 = coord[2][1] + 1
        y2 = coord[2][2] + 1

        if x1 == x2
            println("found horizontal match ", x1, "==", x2, " -> ", y1, ":", y2)
            y1 < y2 ? step = 1 : step = -1
            for n ∈ y1:step:y2
                vents[x1,n] += 1
            end
        elseif y1 == y2
            println("found vertical match ", y1, "==", y2, " -> ", x1, ":", x2)
            x1 < x2 ? step = 1 : step = -1
            for n ∈ x1:step:x2
                vents[n, y1] += 1
            end
        end
    end

    return vents

end

vent_matrix = create_orthogonal_lines(vent_coords)

vent_matrix