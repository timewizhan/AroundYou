//
//  Networking.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 9. 29..
//  Copyright © 2015년 cclab. All rights reserved.
//

import Foundation

struct ST_NETWORK
{
    var socketToServer : Int32 = 0
    var strServerAddress : String = ""
    var nPort : __uint16_t = 0
    var serverAddrIn : sockaddr_in?
}

class CNetworking
{
    var stNetwork = ST_NETWORK()
    
    init()
    {
        stNetwork.strServerAddress = "165.132.120.160"
        stNetwork.nPort = __uint16_t(5432)
        
        let hostAddressArray = stNetwork.strServerAddress.cStringUsingEncoding(NSASCIIStringEncoding)!
        let hostAddr = inet_addr(hostAddressArray)
        stNetwork.socketToServer = socket(AF_INET, SOCK_STREAM, 0)
        stNetwork.serverAddrIn = sockaddr_in(
            sin_len:__uint8_t(sizeof(sockaddr_in)),
            sin_family:sa_family_t(AF_INET),
            sin_port:stNetwork.nPort.bigEndian,
            sin_addr:in_addr(s_addr:hostAddr),
            sin_zero:(0,0,0,0,0,0,0,0))
    }
    
    func connectServer() -> Int32
    {
        let connectResultCode = withUnsafePointer(&stNetwork.serverAddrIn){
            pointer-> Int32 in
            let localSockAddr = UnsafePointer<sockaddr>(pointer)
            return connect(stNetwork.socketToServer, localSockAddr, socklen_t(sizeof(sockaddr_in)))
        }
        return connectResultCode
    }
    
    func sendData(strData : String) -> Int
    {
        let dataToSend : String = strData
        let textCString = dataToSend.cStringUsingEncoding(NSASCIIStringEncoding)
        let textLength = Int(dataToSend.lengthOfBytesUsingEncoding(NSASCIIStringEncoding))
        return send(stNetwork.socketToServer, textCString!, textLength, 0)
    }
    
    func recvData(nSize : Int) -> String
    {
        let receiveSize : Int = nSize
        var receiveBuffer = [CChar](count:Int(receiveSize), repeatedValue: 0)
        recv(stNetwork.socketToServer, &receiveBuffer, receiveSize, 0)
        
        return String.fromCString(receiveBuffer)!
    }
}