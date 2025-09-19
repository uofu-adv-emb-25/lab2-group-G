# Testing Plan
## The plan

---

### How to set up the test scenario
Tests are defined in test.c. This test file utilizes the Unity framework to test our src code. When building, a mytest.elf will be generated and can be used to flash the pico. Connect to tty to see the results of the test.

### How to exercise the system
Not sure yet? I don't know how to call source code with unity yet to test source code.

### Expected behavior

Tests should only print if failing. Otherwise, unity will tell us all tests passed. 