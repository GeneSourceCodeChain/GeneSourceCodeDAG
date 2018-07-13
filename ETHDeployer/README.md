# this project provide an easily deployed eth private chain

### usage
1. start a geth private chain

```bash
bash runserver.sh
```

within the console execute

```bash
loadScript('scripts/boot.js')
```

the miners start to work

2. start a clean console (without disturbance from nagging messages)

```bash
bash console.sh
```

load gene token contract by executing

```bash
loadScript('scripts/set_erc20.js')
```

load dispenser contract by executing

```bash
loadScript('scripts/set_dispenser.js')
```

3. test

test the dispenser by executing

```bash
loadScript('scripts/dispenser.js')
test_dispenser()
```

check the balance by executing

```bash
loadScript('scripts/balance.js')
```

