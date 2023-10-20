includeInsert = "#include \"cron.h\"\n"

begCron = "\tsetupCronometer ();\n\tuint32_t start = ARM_CM_DWT_CYCCNT;\n"

endCron = "\t__uint32_t cycles = ARM_CM_DWT_CYCCNT - start;\n"

def find_last_include_and_main(file_path):
    last_include = None
    main_start = None
    main_end = None

    with open(file_path, 'r') as file:
        lines = file.readlines()

    for i, line in enumerate(lines):
        if line.strip().startswith('#include'):
            last_include = i

        if 'int main(' in line:
            main_start = i

        if main_start is not None and 'return 0;' in line:
            main_end = i

    return last_include, main_start, main_end

def copy_file_line_by_line(input_file, output_file):
    include, mainInit, mainEnd = find_last_include_and_main(input_file)
    include +=1
    mainInit +=1
    if include is not None:
        print(f"Último include na linha {include}:")
    if mainInit is not None and mainEnd is not None:
        print(f"Função main começa na linha {mainInit} e termina na linha {mainEnd}:")

    with open(input_file, 'r') as in_file, open(output_file, 'w') as out_file:
        lines = in_file.readlines()
        for i in range(len(lines)):
            if(i == include):
                out_file.write(includeInsert)
            elif(i == mainInit):
                out_file.write(begCron)
            elif(i == mainEnd):
                out_file.write(endCron)
            out_file.write(lines[i])

if __name__ == '__main__':


    file_path = "./src/main.c"
    copy_file_line_by_line(file_path, file_path+"e")



    