//
//  MainPageContainerController.swift
//  AroundYou
//
//  Created by 물통꿀꿀이 on 2015. 12. 13..
//  Copyright © 2015년 cclab. All rights reserved.
//

import UIKit

class MainPageContainerController: UIViewController {

    @IBOutlet weak var imageRecommendedStore: UIImageView!
    @IBOutlet weak var imageRecommendedMenu: UIImageView!
    @IBOutlet weak var imageRank: UIImageView!
    @IBOutlet weak var imageLeftArrow: UIImageView!
    @IBOutlet weak var imageRightArrow: UIImageView!
    @IBOutlet weak var imageMainStore: UIImageView!
    
    var flagRecommendedStore : Bool = true
    var flagRecommendedMenu : Bool = false
    
    var currentPage : Int = 1
    let firstPage : Int = 1
    let lastPage : Int = 1
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        setGestureToImages()
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func setGestureToImages() {
        let gestureClickedRecommendedStore = UITapGestureRecognizer(target: self, action: "clickedRecommendedStore:")
        imageRecommendedStore.addGestureRecognizer(gestureClickedRecommendedStore)
        imageRecommendedStore.userInteractionEnabled = true

        let gestureClickedRecommencedMenu = UITapGestureRecognizer(target: self, action: "clickedRecommendedMenu:")
        imageRecommendedMenu.addGestureRecognizer(gestureClickedRecommencedMenu)
        imageRecommendedMenu.userInteractionEnabled = true
        
        let gestureClickedLeftArrow = UITapGestureRecognizer(target: self, action: "clickedLeftArrow:")
        imageLeftArrow.addGestureRecognizer(gestureClickedLeftArrow)
        imageLeftArrow.userInteractionEnabled = true
        
        let gestureClickedRightArrow = UITapGestureRecognizer(target: self, action: "clickedRightArrow:")
        imageRightArrow.addGestureRecognizer(gestureClickedRightArrow)
        imageRightArrow.userInteractionEnabled = true
        
        // click main view
        let gestureClickedMainStore = UITapGestureRecognizer(target: self, action: "clickedMainStore:")
        imageMainStore.addGestureRecognizer(gestureClickedMainStore)
        imageMainStore.userInteractionEnabled = true
        
    }
    
    func clickedRecommendedStore(sender : AnyObject) {
        debugPrint("Clicked recommended store")
        
        if flagRecommendedStore == false {
            flagRecommendedMenu = false
            flagRecommendedStore = true
            
            // able RecommendedStore
            imageRecommendedStore.image = UIImage(named: "page1_028.png")
            
            // diable RecommendedMenu
            imageRecommendedMenu.image = UIImage(named: "page1_031.png")
        }
    }
    
    func clickedRecommendedMenu(sender : AnyObject) {
        debugPrint("Clicked recommended menu")
        
        if flagRecommendedMenu == false {
            flagRecommendedMenu = true
            flagRecommendedStore = false
            
            // able RecommendedMenu
            imageRecommendedMenu.image = UIImage(named: "page1_030.png")
            
            // disable RecommendedStore
            imageRecommendedStore.image = UIImage(named: "page1_029.png")
        }
    }
    
    func clickedLeftArrow(sender : AnyObject) {
        debugPrint("Click recommended left arrow")
    }

    func clickedRightArrow(sender : AnyObject) {
        debugPrint("Click recommended right arrow")
    }
    
    func clickedMainStore(sender : AnyObject) {
        debugPrint("Click main image")
        
        performSegueWithIdentifier("test", sender: self)
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
