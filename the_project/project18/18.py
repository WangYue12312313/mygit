
# Replace with your testnet node credentials
rpc_user = "your_rpc_username"
rpc_password = "your_rpc_password"
rpc_port = 18332

# Connect to the testnet node
rpc_connection = AuthServiceProxy(f"http://{rpc_user}:{rpc_password}@127.0.0.1:{rpc_port}")

def send_testnet_transaction():
    # Replace with the testnet address you want to send funds to
    to_address = "n3x6j4fjPL7NVmGouo8GA62SQLEjvKdbKV"

    # Create a raw transaction
    raw_transaction = rpc_connection.createrawtransaction([], {to_address: 0.001})

    # Sign the raw transaction
    signed_transaction = rpc_connection.signrawtransactionwithwallet(raw_transaction)

    # Send the signed transaction
    tx_id = rpc_connection.sendrawtransaction(signed_transaction['hex'])

    return tx_id

def parse_transaction_data(transaction_id):
    # Get the raw transaction data
    raw_transaction = rpc_connection.getrawtransaction(transaction_id, 1)

    # Decode the raw transaction to get each field
    tx_data = {
        "version": raw_transaction["version"],
        "size": len(raw_transaction["hex"]) / 2,  # Size in bytes
        "vin": [input["txid"] for input in raw_transaction["vin"]],
        "vout": [output["value"] for output in raw_transaction["vout"]],
        "locktime": raw_transaction["locktime"],
    }

    return tx_data

if __name__ == "__main__":
    try:
        # Send a testnet transaction
        transaction_id = send_testnet_transaction()
        print("Transaction sent. Transaction ID:", transaction_id)

        # Parse the transaction data
        tx_data = parse_transaction_data(transaction_id)
        print("\nTransaction Data:")
        for key, value in tx_data.items():
            print(f"{key}: {value}")

    except JSONRPCException as e:
        print("Error:", e.error['message'])
