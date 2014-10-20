public synchronized void increment() {
    c++; //1
}

public synchronized void decrement() {
    c--; //2
}

synchronized(x) {
    c--;// 3
}

c--;
