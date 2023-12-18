//
//  BLEManager.swift
//  ARMS
//
//  Created by Radwan Alrefai on 12/17/23.
//  BLE Core Class for function related to BLE connection with the smart band
//

import Foundation
import CoreBluetooth

class BLEManager: NSObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        <#code#>
    }
    
    static let shared = BLEManager()
    
    private var centralManager: CBCentralManager?
    private var peripheral: CBPeripheral?
    
    override init() {
        super.init()
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }
    
    
}
