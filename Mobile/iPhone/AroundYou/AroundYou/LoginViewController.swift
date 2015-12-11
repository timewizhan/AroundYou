//
//  LoginViewController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 9. 29..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit
import CoreTelephony

class LoginViewController: UIViewController {
    @IBOutlet weak var imageForDrag: UIImageView!
    @IBOutlet weak var imagebarForDrag: UIImageView!
    
    var imagePosition : CGPoint = CGPoint(x: 0, y: 0)
    var imageLimitedMinX : CGFloat = 0
    var imageLimitedMaxX : CGFloat = 0
    var touchedImage : Bool = false
    var loginToPermit : Bool = false
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        //NSThread.sleepForTimeInterval(3)
        // Do any additional setup after loading the view.
        
        // To get data of image size
        findCurrentImagePosition()
        findImageBarPosition()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    override func touchesBegan(touches: Set<UITouch>, withEvent event: UIEvent?) {
        if let touch : UITouch = touches.first! {
            let positionCurrentTouch = touch.locationInView(self.view)
            
            let touchXPosition = positionCurrentTouch.x
            let touchYPosition = positionCurrentTouch.y
            
            let rectImage : CGRect = imageForDrag.frame
            let maxXPosition = rectImage.origin.x + rectImage.width
            let minXPosition = rectImage.origin.x
            let maxYPosition = rectImage.origin.y + rectImage.height
            let minYPosition = rectImage.origin.y
            
            if touchXPosition > minXPosition && touchXPosition < maxXPosition {
                if touchYPosition > minYPosition && touchYPosition < maxYPosition {
                    // touch image for drag
                    touchedImage = true
                    debugPrint("Touch ImageView")
                }
            }
        }
    }
    
    override func touchesMoved(touches: Set<UITouch>, withEvent event: UIEvent?) {
        if touchedImage != true {
            return
        }
        
        if let touch : UITouch = touches.first! {
            let positionCurrentTouch = touch.locationInView(self.view)
            
            let touchXPosition = positionCurrentTouch.x
            let rectImage : CGRect = imageForDrag.frame
            
            let positionToMove = touchXPosition + rectImage.width / 2
            if positionToMove < imageLimitedMinX || positionToMove > imageLimitedMaxX {
                return
            }
            
            imagePosition.x = positionToMove
            imageForDrag.center = imagePosition
            
            if imagePosition.x >= imageLimitedMaxX - 50 && imagePosition.x <= imageLimitedMaxX {
                loginToPermit = true
                debugPrint("Login to permit")
                
                moveToNextView()
            }
        }
    }
    
    override func touchesEnded(touches: Set<UITouch>, withEvent event: UIEvent?) {
        if touchedImage == true {
            touchedImage = false
            
            // if user end touch, the image have to returned to first position
            imagePosition.x = imageLimitedMinX
            imageForDrag.center = imagePosition
        }
    }
    
    func findCurrentImagePosition() {
        let rectImage : CGRect = imageForDrag.frame
        let centerX = rectImage.width / 2 + rectImage.origin.x
        let centerY = rectImage.height / 2 + rectImage.origin.y
        
        imagePosition = CGPoint(x: centerX, y: centerY)
    }
    
    func findImageBarPosition() {
        let rectImage : CGRect = imagebarForDrag.frame
        imageLimitedMinX = rectImage.origin.x + imageForDrag.frame.width / 2
        imageLimitedMaxX = rectImage.origin.x + rectImage.width
    }

    func moveToNextView() {
        self.performSegueWithIdentifier("completedLogin", sender: nil)
    }
    
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
