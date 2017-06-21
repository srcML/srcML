
if(x) {
    break /* 1 - 3*/;
}

do {
    break /* 4 - 5*/;
} while(something);


do {
    switch(x) {
        case 1:
            break /* 6 - 7 */;
        default:
    }
} while(something);

do {
    for(int x = 0; x < 0; ++x) {
        while(x) {
            synchronized(x) {
                break /* 8 - 9 */;
            }
        }
    }
} while(something);
