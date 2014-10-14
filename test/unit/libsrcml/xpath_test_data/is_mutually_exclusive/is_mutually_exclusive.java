public synchronized void increment() {
    c++;
}

public synchronized void decrement() {
    c--;
}

synchronized(x) {
    c--;
}

c--;
