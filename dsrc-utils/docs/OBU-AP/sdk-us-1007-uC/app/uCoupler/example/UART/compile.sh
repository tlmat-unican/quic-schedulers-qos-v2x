rm uart_sender uart_receiver
gcc uart_sender.c minihdlc.c -o uart_sender
gcc uart_receiver.c minihdlc.c -o uart_receiver
