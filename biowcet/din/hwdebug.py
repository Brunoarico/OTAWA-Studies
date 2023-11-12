import gdb
import tqdm

n_measures = 10

dados_x = []
dados_y = []

measure = list()

file_name = "src/main.c"
def mean(array):
    if not array:
        return 0  
    m = sum(array)/len(array)  
    return m

def findLineCycles():
    with open(file_name, 'r') as arquivo:
        linhas = arquivo.readlines()
        nlinhas = len(linhas)
        for i in (range(nlinhas)):
            if("cycles" in linhas[i+1]):
                #print(f"Linha {i+2}: {linhas[i+2].strip()}")
                return i+3
    return -1


def main():
    gdb.execute('set confirm off') #disable quit confirmation
    gdb.execute('set pagination off')  # disable GDB pagination
    gdb.execute('target remote localhost:3333')
    breakp = findLineCycles()
    major = 0
    #for i in range(0, n_measures):
    for i in tqdm.tqdm(range(0, n_measures)):
        a = gdb.execute('monitor reset halt', to_string=True)
        b = gdb.execute(f'break {breakp}', to_string=True)
        c = gdb.execute('c',to_string=True)
        cycles_value = int(gdb.parse_and_eval('cycles'))
        #gdb.execute('print cycles')
        #gdb.execute('print arr')
        major = max(major, cycles_value)
        print(major)
        measure.append(cycles_value)
        

if __name__ == '__main__':
    main()
    print('Wcet_Dynamic: ', max(measure))
    gdb.execute('quit')
    
