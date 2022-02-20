def simple_extender(synonyms, total=5):
    with open("complex_cfg_source.txt") as file:
        input_str = " ".join(file.readlines())

    res = [input_str]
    for n in range(total):
        new_str = input_str
        for i in range(len(synonyms)):
            if synonyms[i][-2:] != "00":
                synonyms[i] = synonyms[i] + "00"

            new_str = new_str.replace(synonyms[i], synonyms[i][:-1] + str(n + 1))
        res.append(new_str)
    with open("output.txt", "w") as output_file:
        output_file.writelines(res)

def extendTuple(synonyms, num_lines, total, tuples):
    copy = tuples.copy()
    for k in range(len(tuples)):
        synonymCopy = synonyms.copy()
        for n in range(total):
            temp = []
            for j in range(len(tuples[k])):
                if tuples[k][j].isdigit():
                    temp.append(str(int(tuples[k][j]) + (num_lines * (n + 1))))
                else:
                    if tuples[k][j][-2] != "0":
                        copyTuple = tuples[k][j][:-2] + str(n + 1)
                    else:    
                        copyTuple = tuples[k][j][:-1] + str(n + 1)
                    temp.append(tuples[k][j][:-1] + str(n + 1))
            copy.append(temp)
    copy = list(map(lambda x: ' '.join(x), copy))
    return ', '.join(copy) + ", "
                    
def query_extender(synonyms, num_lines, total=5):
    for i in range(len(synonyms)):
        if synonyms[i][-2:] != "00":
            synonyms[i] = synonyms[i] + "00"
    with open("complex_cfg_queries.txt") as input_file:
        lines = input_file.readlines()
        res = ["".join(lines)]
        new_lines = lines.copy()
        for j in range(3, len(lines), 5):
            if "none" in lines[j] or "FALSE" in lines[j] or "TRUE" in lines[j]:
                continue
            ans = new_lines[j][:-1] + ", "
            synonymCopy = synonyms.copy()
            arr = new_lines[j].split(",")
            arr = list(map(lambda x: x.split(), arr))
            if len(arr[0]) > 1:
                ans = extendTuple(synonymCopy, num_lines, total, arr)
                if not ans: ans = "none"
            else:
                for n in range(total):
                    for i in range(len(synonyms)):
                        if synonymCopy[i][-2] != "0":
                            new_lines[j] = (new_lines[j]).replace(synonymCopy[i], synonymCopy[i][:-2] + str(n + 1))
                            synonymCopy[i] = synonymCopy[i][:-2] + str(n + 1)
                        else:    
                            new_lines[j] = (new_lines[j]).replace(synonymCopy[i], synonymCopy[i][:-1] + str(n + 1))
                            synonymCopy[i] = synonymCopy[i][:-1] + str(n + 1)
                        
                    new_lines[j] = new_lines[j].replace(" ", "")
                    arr = new_lines[j].split(",")
                    if arr[0].isdigit() or len(arr) == 1 and arr[0][:-1].isdigit():
                        arr[-1] = arr[-1][:-1]
                        for idx in range(len(arr)):
                            arr[idx] = str(int(arr[idx]) + int(num_lines * (n + 1)))     
                    arr[-1] += ", "
                    ans += (", ".join(arr))
                    ans = ans.replace("\n", "")
            lines[j] = ans[:-2] + "\n"
            
    res = "".join(lines)
    with open("output_query.txt", "w") as output_file:
        output_file.writelines(res)

def cloner(synonyms, num_lines, total=5):
    simple_extender(synonyms,total)
    query_extender(synonyms, num_lines, total)



if __name__ == "__main__":
    # input to cloner are the keywords being used in the code - All variable names and procedure names
    # currently its set for complex_cfg_source.txt and complex_cfg_queries.txt
    # Simply place the source and query file in the same directory as this .py file and run
    cloner(["main", "k", "x", "l", "r", "a", "b", "k", "z", "y", "cr", "s", "xy"], 27, 5)

