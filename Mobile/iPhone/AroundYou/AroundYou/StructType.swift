//
//  StructType.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 5..
//  Copyright © 2015년 cclab. All rights reserved.
//

import Foundation

struct Comment
{
    var userId: String?
    var strComment: String?
    var strStar: String?
    
    init(userId: String?, strComment: String?, strStar: String?) {
        self.userId = userId
        self.strComment = strComment
        self.strStar = strStar
    }
}

struct UserAccount
{
    static var strUserNickName : String?
    static var strUserEmail : String?
    static var strCallID : String?
    
    /*
        set function
    */
    static func setUserNickName(UserNickName : String) {
        strUserNickName = UserNickName
    }
    
    static func setUserEmail(UserEmail : String) {
        strUserEmail = UserEmail
    }
    
    static func setCallID(CallID : String) {
        strCallID = CallID
    }
    /*
        get function
    */
    static func getUserNickName() -> String {
        return strUserNickName!
    }
    
    static func getUserEmail() -> String {
        return strUserEmail!
    }
    
    static func getCallID() -> String {
        return strCallID!
    }
}

struct CurrentClickedValue
{
    static var strStoreName : String?
    static var strStoreReputation : String?
    static var strStoreMenu : String?
    
    /*
        set function
    */
    static func setStoreName(StoreName : String) {
        strStoreName = StoreName
    }
    
    static func setStoreReputation(StoreReputation : String) {
        strStoreReputation = StoreReputation
    }
    
    static func setStoreMenu(StoreMenu : String) {
        strStoreMenu = StoreMenu
    }
    /*
        get function
    */
    static func getStoreName() -> String {
        return strStoreName!
    }
    
    static func getStoreReputation() -> String {
        return strStoreReputation!
    }
    
    static func getStoreMenu() -> String {
        return strStoreMenu!
    }
}