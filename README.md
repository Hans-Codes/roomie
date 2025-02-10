# Roomie

Roomie is a broadcast client made in C

## Installation
For now, as of Version 1.00 there's still a lot of bugs so I wont make a release just yet.. However you can try this application by compiling it on a **Linux** machine. Since I'm using a UNIX library. Make sure you have gcc installed

```bash
gcc -o client client.c && gcc -o server server.c && gcc -o host host.c
```

That should compile everything, be sure to change the IP in all the source codes.

## Usage
First of all, just to make sure you need to change the IP to your IP.

There are three source codes:
- client.c
- server.c
- host.c

client.c doesn't receive any inputs, will only receive the broadcasts.

server.c acts as a middleman between client.c and host.c

host.c used to send broadcasts (needs a login).

## TODO
- Fix login auth (auth isnt working yet)
- Fix UI on host side
- Make it so this is usable in Windows
- Add logging on server side
- Add some control on server side (for example making a host account in the server without having to create the file yourself)
- use JSON instead of txt
- Server side password hashing

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
