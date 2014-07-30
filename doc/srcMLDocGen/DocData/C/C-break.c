char c;
for(;;) {
    printf_s( "\nPress any key, Q to quit: " );

    // Convert to character value
    scanf_s("%c", &c);
    if (c == 'Q')
        break;
}