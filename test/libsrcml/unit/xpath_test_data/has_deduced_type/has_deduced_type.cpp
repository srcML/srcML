auto i = 5; // 1 & 2

auto foo(); // 3

auto foo()->otherType; // 4

auto foo() { } // 5

auto foo()->otherType { } // 6

temp = []()-> auto { }; // 7
