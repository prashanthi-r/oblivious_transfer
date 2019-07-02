rsa.h contains common functions required for rsa implementation using gmp

strings.txt contains the two messages, the first corresponding to Bob selecting 0 and the second corresponding to him selecting 1.

1. run keygen.c (executable --> keys) to generate e,d and n

public_key.txt contains n in the first line and e in the second line
private_key.txt contains n in the first line and d in the second line

2. run alice_ot1.c (executable --> alice1) to generate x_0 and x_1

randomx.txt is the file to store randomly generated x_0 and x_1 (on separate lines).

3. run bob_ot1.c (executable --> bob1) to generate a random number k, input bit b, and calculate c --> c = (x_b + k^e) mod n

bob_c.txt is the file where c is stored
bob_private.txt is the file that stores the random number k and the bit b 


4. now run alice_ot2.c (executable --> alice2) to calculate v_0 (v_0 = (c – x_0)^d mod n) and v_1 (v_1 = (c – x_0)^d mod n) and encrypted messages m’_0 and m’_1 such that m’_0 = (m_0 – v_0) and m’_1 = (m_1 – v_1)

strings.txt contains two strings (m_0 and m_1, one of which is to be shared by oblivious transfer)
enc_messages.txt should contain m'_0 (m'_0 = m_0 - v_0) and m'_1 (m'_1 = m_1 - v_1)
the executable for alice_ot2.c is alice2

5. now run bob_ot2.c (executable --> bob2) to get the final message, m_b (m_b = m’_b + k)

bob_message.txt contains the final message that Bob receives
the executable for bob_ot2.c is bob2


Instructions to use remote OT Protocol

Server:-

Add the two numbers that are to be chosen from to strings.txt.

run alice_server from the command line of the device that needs to send the numbers.

A port number should be provided as a command line argument.

Example

	./alice_server <port>

This will allow the program to accept connections on port number <port> of the device.

Client:-

Run bob_client from the command line of the device that needs to receive the numbers.

The ip address of the device running the server and the port number it as accepting connections on should be provided as comand line arguments.

Example

	./bob_client <ip address> <port>

This will allow the program to connect to the given port of the given ip address.

ip address should be 127.0.0.1 when both the connections are being run on the same device.

Both server and clients will be asked to input seeds for random number generation.

The client will be asked to enter b (either 0 or 1) which will indicate their choice.
