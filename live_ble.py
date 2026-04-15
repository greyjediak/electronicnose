import asyncio
from bleak import BleakClient, BleakScanner

UART_TX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

def handle_data(sender, data):
    text = data.decode("utf-8", errors="ignore").strip()
    print("RX:", text)

async def main():
    print("Scanning...")
    devices = await BleakScanner.discover()

    target = None
    for d in devices:
        print(f"Found: {d.name} | {d.address}")
        if d.name == "Acetone-XIAO":
            target = d
            break

    if not target:
        print("Device not found.")
        return

    print(f"Connecting to {target.name} at {target.address}...")

    async with BleakClient(target.address) as client:
        print("Connected:", client.is_connected)

        services = client.services
        print("\nServices and characteristics:")
        for service in services:
            print(f"[Service] {service.uuid}")
            for char in service.characteristics:
                print(f"  [Char] {char.uuid} | props={char.properties}")

        print("\nStarting notify...")
        await client.start_notify(UART_TX_UUID, handle_data)
        print("Watching live BLE output. Press Ctrl+C to stop.\n")

        while True:
            await asyncio.sleep(1)

asyncio.run(main())