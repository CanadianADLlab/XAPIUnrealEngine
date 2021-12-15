
<h1> How to call statement via C++</h1>
Add the XAPIManger to a level (Can be found if you search for it in the PlaceActors area)
<br>
<br>
<img width="508" alt="image1" src="https://user-images.githubusercontent.com/39784801/136555156-cd2dcceb-66b4-40f4-a42c-203635f8d033.png">
<br>




Now set up the endpoint and the basic auth key which can be found in your lrs settings.
The end point HAS TO BE THE url/statments so for example it would be https://insertmylrshere.net/data/xAPI/statements, in the unity package the url I used was https://insertmylrshere.net/data/xAPI causing some confusion here for myself.
<br>
<br>

<img width="365" alt="image2" src="https://user-images.githubusercontent.com/39784801/136555175-5725d898-bb39-4b46-ba8a-6202b1e396fa.png">
<br>

To reference in C++ just add the XAPIManger as a UProperty and set it via the inspector
<br>
<br>

<img width="258" alt="iamge3" src="https://user-images.githubusercontent.com/39784801/136555190-f52f98ad-2e6f-43b8-8273-5a18991f2501.png">

<br>

Than call the CreateXAPIPhrase function to send info to the lrs
<br>
<br>

![newxapiunrealscreenshot](https://user-images.githubusercontent.com/39784801/146002697-05cc509c-3357-4aef-95ca-ff4f85d92b23.png)

<h1>Call XAPI Statements via blueprints</h1>

Add the XAPIManger to a level (Can be found if you search for it in the PlaceActors area)
<br>
<br>
<img width="508" alt="image1" src="https://user-images.githubusercontent.com/39784801/136555156-cd2dcceb-66b4-40f4-a42c-203635f8d033.png">
<br>

Now to call the CreateXAPIStatement via blueprints, find the xapi manager by using the node GetActorOfClass

<br>

![getactorofclass](https://user-images.githubusercontent.com/39784801/146206511-6fb0b8f9-f135-48a5-a323-350a7e28780d.png)

<br>

Now with a reference to the XAPIManger you can call the CreateXAPIPhrase node which looks like this.

<br>



![createphrase](https://user-images.githubusercontent.com/39784801/146208837-582033f8-f7c3-4afb-8f34-c7f145785bf8.png)

