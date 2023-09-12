

int main () {
    int i = 0;
    int sum = 0;
    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            sum += i;
        } 
        else {
            sum += i*2;
        }
    }
    return 0;
}
