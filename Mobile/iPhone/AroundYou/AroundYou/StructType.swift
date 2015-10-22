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
    var strUserNickName : String?
    var strUserEmail : String?
    
    init(strUserNickName : String?, strUserEmail : String?) {
        self.strUserNickName = strUserNickName
        self.strUserEmail = strUserEmail
    }
}

struct ViewValueState
{
    var strStoreName : String?
    var strStoreReputation : String?
    var strStoreMenu : String?
}