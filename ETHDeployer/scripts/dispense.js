function assert(condition, message) {
	if(!condition) {
		message = message || "Assertion failed";
		if(typeof Error != "undefined") {
			throw new Error(message);
		}
		throw message;
	}
}

function dispense(from,to,_gas,_gasPrice)
{
	if(typeof(_gas) === 'undefined') _gas = 700000;
	if(typeof(_gasPrice) === 'undefined') _gasPrice = web3.toWei(90,'gwei');
	assert(true == personal.unlockAccount(from['publickey'],from['password']),"can't unlock wallet!");
	var contract = eth.contract(
		[{"constant":true,"inputs":[],"name":"name","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"spender","type":"address"},{"name":"tokens","type":"uint256"}],"name":"approve","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"totalSupply","outputs":[{"name":"","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"from","type":"address"},{"name":"to","type":"address"},{"name":"tokens","type":"uint256"}],"name":"transferFrom","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"decimals","outputs":[{"name":"","type":"uint8"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[{"name":"tokenOwner","type":"address"}],"name":"balanceOf","outputs":[{"name":"balance","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[],"name":"acceptOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"owner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":true,"inputs":[],"name":"symbol","outputs":[{"name":"","type":"string"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"to","type":"address"},{"name":"tokens","type":"uint256"}],"name":"transfer","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":false,"inputs":[{"name":"spender","type":"address"},{"name":"tokens","type":"uint256"},{"name":"data","type":"bytes"}],"name":"approveAndCall","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[],"name":"newOwner","outputs":[{"name":"","type":"address"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"tokenAddress","type":"address"},{"name":"tokens","type":"uint256"}],"name":"transferAnyERC20Token","outputs":[{"name":"success","type":"bool"}],"payable":false,"stateMutability":"nonpayable","type":"function"},{"constant":true,"inputs":[{"name":"tokenOwner","type":"address"},{"name":"spender","type":"address"}],"name":"allowance","outputs":[{"name":"remaining","type":"uint256"}],"payable":false,"stateMutability":"view","type":"function"},{"constant":false,"inputs":[{"name":"_newOwner","type":"address"}],"name":"transferOwnership","outputs":[],"payable":false,"stateMutability":"nonpayable","type":"function"},{"inputs":[],"payable":false,"stateMutability":"nonpayable","type":"constructor"},{"payable":true,"stateMutability":"payable","type":"fallback"},{"anonymous":false,"inputs":[{"indexed":true,"name":"_from","type":"address"},{"indexed":true,"name":"_to","type":"address"}],"name":"OwnershipTransferred","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"from","type":"address"},{"indexed":true,"name":"to","type":"address"},{"indexed":false,"name":"tokens","type":"uint256"}],"name":"Transfer","type":"event"},{"anonymous":false,"inputs":[{"indexed":true,"name":"tokenOwner","type":"address"},{"indexed":true,"name":"spender","type":"address"},{"indexed":false,"name":"tokens","type":"uint256"}],"name":"Approval","type":"event"}]
	).at('0xaa144434bd498180a58ec9ad8a7687e113978f40');
	var start = Date.now();
	for(var i = 0 ; i < to.length ; i++) {
		contract.transfer.sendTransaction(to[i]['publickey'],to[i]['quantity'],{from:from['publickey'],gas:_gas,gasPrice:_gasPrice});
	}
	console.log((Date.now() - start) + "ms");
}

function test_dispense()
{
	dispense(
		{'publickey':eth.accounts[0],'password':'genesourceco'},
		[
			{'publickey':eth.accounts[1],'quantity':1},
			{'publickey':eth.accounts[2],'quantity':1},
			{'publickey':eth.accounts[3],'quantity':1},
			{'publickey':eth.accounts[1],'quantity':1},
			{'publickey':eth.accounts[2],'quantity':1},
			{'publickey':eth.accounts[3],'quantity':1},
			{'publickey':eth.accounts[1],'quantity':1},
			{'publickey':eth.accounts[2],'quantity':1},
			{'publickey':eth.accounts[3],'quantity':1},
			{'publickey':eth.accounts[1],'quantity':1},                                                                                                                           {'publickey':eth.accounts[2],'quantity':1},                                                                                                                           {'publickey':eth.accounts[3],'quantity':1},                                                                                                                           {'publickey':eth.accounts[1],'quantity':1},                                                                                                                           {'publickey':eth.accounts[2],'quantity':1},                                                                                                                           {'publickey':eth.accounts[3],'quantity':1},                                                                                                                           {'publickey':eth.accounts[1],'quantity':1},                                                                                                                           {'publickey':eth.accounts[2],'quantity':1},                                                                                                                           {'publickey':eth.accounts[3],'quantity':1},
		]
	);
}

