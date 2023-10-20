
//iterative factorial
int fat (int n) {
    int i = 1;
    int fat = 1;
    while (i <= n) {
        fat = fat * i;
        i = i + 1;
    }
    return fat;
}

int main () {
    int b = 20;
    int a = 10;

    int sa = fat (a);
    int sb = fat (b);

    return 0;

}


