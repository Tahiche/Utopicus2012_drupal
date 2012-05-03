// JavaScript Document

CKEDITOR.addStylesSet('my_styles',
    [
      {
        name: 'Spaced UL list',
        element: 'ul',
        attributes: {'class' : 'spaced'}
      },
      {
        name: 'Spaced OL list',
        element: 'ol',
        attributes: {'class' : 'spaced'}
      },
	  // Block Styles
	  { name : 'Ninguno', element : 'p' },
	 { name : 'H2', element : 'h2', attributes : { 'class' : 'content' } },
	 { name : 'H3', element : 'h3'},
	 { name : 'Box Div', element : 'div', attributes : { 'class' : 'block block-boxes' } },
      
      {
        name: 'Boxes - Cajas',
        element: 'ol',
        attributes: {'class' : 'block block-boxes'}
      },
	 
   /* { name : 'Blue Title', element : 'h2', styles : { 'color' : 'Blue' } },
    { name : 'Red Title' , element : 'h3', styles : { 'color' : 'Red' } },

    // Inline Styles
    { name : 'CSS Style', element : 'span', attributes : { 'class' : 'my_style' } },
    { name : 'Marker: Yellow', element : 'span', styles : { 'background-color' : 'Yellow' } }*/
    ]
    ); 