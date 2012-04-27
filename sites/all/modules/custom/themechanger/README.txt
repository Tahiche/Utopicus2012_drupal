// The function get_remote_image_from_url($url) takes a url: http://www.example.com/projects/image1235.jpg
// From that url it downloads the image and stores it in your 'files' directory in the folder 'remote_images'.
// It then makes the image available to you via a local path (eg: sites/default/files/remote_images/projects-image1235.jpg in this case).
// You can always reference the 'url' and it will return the local image if it is already stored, or it will get the image and return the local path.
// This function will return the local path of the image, or "" if there was an error getting the image.
// For usage with imagecache, use the following, replacing 'present_name' with the imagecache present you want to use.
//   theme('imagecache', 'present_name', get_remote_image_from_url("http://www.example.com/projects/image1235.jpg"));



// When you want to place the remote image on the page with imagecache using the 'present_name' present, use the following line of code.
$output = theme('imagecache', 'present_name', get_remote_image_from_url("http://www.example.com/projects/image1235.jpg"));


// Assuming you were going to do it inline with other content.
$output = '';
$output .= '<div class="image-wrapper">';
$output .= theme('imagecache', 'present_name', get_remote_image_from_url("http://www.example.com/projects/image1235.jpg"));
$output .= '</div>';
echo $output;