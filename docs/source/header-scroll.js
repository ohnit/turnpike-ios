// Hide & Show Header JS
var psY = 0;
var sY = 0;

window.addEventListener('scroll', function(){
    psY = sY;
    sY = window.pageYOffset;
    // console.log("scrollY: " + sY);

    if(sY <= document.body.offsetHeight/4) {
      $("#header").removeClass("header-hide");
    }
    else {
      var scrollDirection = (sY - psY);
      if(scrollDirection > 0) {
        $("#header").addClass("hide-header");
      }
      else if(scrollDirection < 0) {
        $("#header").removeClass("hide-header");
      }
    }
   }, false);

window.addEventListener('resize', function() {
  $("#header").removeClass("hide-header");
});
