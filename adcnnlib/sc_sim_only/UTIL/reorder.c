#include <stdio.h>

main ()
{
  int output_tensor_height, output_tensor_width, layer_size;
  int layer_size_x, layer_size_y;
  int i, j, k, l;
  int x, y, h, w;
  int m, n;

  int output_tensor[290400];
  int int_output;

  output_tensor_height = 55;
  output_tensor_width = 55;
  layer_size_x = 12;
  layer_size_y = 8;
  layer_size = layer_size_x * layer_size_y;


  for (h = 0; h < output_tensor_height; h++)
    {
      for (w = 0; w < output_tensor_width; w++)
	{
	  for (y = 0; y < layer_size_y; y++)	// 8
	    {
	      for (x = 0; x < layer_size_x; x++)	// 12
		{

		  m =
		    (x * output_tensor_width) +
		    (y * output_tensor_height * layer_size_x *
		     output_tensor_width) + w +
		    (h * layer_size_x * output_tensor_width);
		  //printf("m = %d\n",m);
		  read (0, &output_tensor[m], 1);

		}
	    }
	}
    }

  for (i = 0; i < 290400; i++)
    {
      int_output = output_tensor[i];
      write (1, &int_output, 1);
    }

}
