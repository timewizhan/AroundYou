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
    
    @IBOutlet weak var webViewAboutImage: UIWebView!
    @IBOutlet var viewParentAboutwebView: UIView!
    
    var flagRecommendedStore : Bool = true
    var flagRecommendedMenu : Bool = false
    
    var currentPage : Int = 1
    let firstPage : Int = 1
    let lastPage : Int = 1
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        setGestureToImages()
        loadWebURL()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func loadWebURL() {
        let webURL = NSURL(string: "http://cfile27.uf.tistory.com/image/2116DA485370846738F840")
        let urlRequest = NSURLRequest(URL: webURL!)
        webViewAboutImage.loadRequest(urlRequest)
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
        let gestureClickedMainImage = UITapGestureRecognizer(target: self, action: "clickedMainImage:")
        viewParentAboutwebView.addGestureRecognizer(gestureClickedMainImage)
        viewParentAboutwebView.userInteractionEnabled = true
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
            
            let webURL = NSURL(string: "http://cfile27.uf.tistory.com/image/2116DA485370846738F840")
            let urlRequest = NSURLRequest(URL: webURL!)
            webViewAboutImage.loadRequest(urlRequest)
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
            
            let webURL = NSURL(string: "http://cfile236.uf.daum.net/image/2102834052D29C8202FF9F")
            let urlRequest = NSURLRequest(URL: webURL!)
            webViewAboutImage.loadRequest(urlRequest)
        }
    }
    
    func clickedLeftArrow(sender : AnyObject) {
        debugPrint("Click recommended left arrow")
    }

    func clickedRightArrow(sender : AnyObject) {
        debugPrint("Click recommended right arrow")
    }
    
    func clickedMainImage(sender : AnyObject) {
        debugPrint("Click main image")
        performSegueWithIdentifier("SelectPageSegue", sender: self)
    }
    
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        
        if segue.identifier == "SelectPageSegue" {
            let selectPageViewController = segue.destinationViewController as! SelectPageViewController
            if flagRecommendedStore == true {
                selectPageViewController.requestedPage = E_BUTTON_TYPE.E_BUTTON_SELECT_DETAIL_STORE_PAGE
            }
            else if flagRecommendedMenu == true {
                selectPageViewController.requestedPage = E_BUTTON_TYPE.E_BUTTON_SELECT_DETAIL_MENU_PAGE
            }
            else {
                debugPrint("Invalid segue flag")
                // nothing
            }
        }
    }
    

}
