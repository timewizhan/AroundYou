//
//  CommentCell.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 10. 5..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class CommentCell: UITableViewCell {

    @IBOutlet weak var userIDLabel: UILabel!
    @IBOutlet weak var starImage: UIImageView!
    @IBOutlet weak var textComment: UITextField!
    
    var comment: Comment! {
        didSet {
            userIDLabel.text = comment.userId
            starImage.image = imageForRating(Int(comment.strStar!)!)
            textComment.text = comment.strComment
        }
    }
    
    func imageForRating(rating:Int) -> UIImage? {
        let imageName = "star\(rating)"
        return UIImage(named: imageName)
    }
}