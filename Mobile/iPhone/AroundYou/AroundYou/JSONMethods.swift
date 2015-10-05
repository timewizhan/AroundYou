//
//  JSONMethods.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 2..
//  Copyright © 2015년 cclab. All rights reserved.
//

import Foundation

class JSONMethods
{
    func convertStringToDictionary(strJSON: String) -> NSDictionary? {
        if let data = strJSON.dataUsingEncoding(NSUTF8StringEncoding) {
            do
            {
                let dicJSON = try  NSJSONSerialization.JSONObjectWithData(data, options: NSJSONReadingOptions.MutableContainers) as? NSDictionary
                return dicJSON
            } catch let error as NSError {
                print(error)
            }
        }
        return nil
    }
}