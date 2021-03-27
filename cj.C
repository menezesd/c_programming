#include <iostream>
#include <string>
#include <vector>
using namespace std;

int
main ()
{

  int T;
  cin >> T;
  for (int tt = 1; tt <= T; tt++)
    {
      int ans;
      string s;
      int x, y;

      cin >> x >> y;
      cin >> s;

      char prev;

      int i;

      cerr << s << '\n';

      for (i = 0; i < s.size (); i++)
	{

	  if (s[i] == 'C' || s[i] == 'J')
	    {
	      prev = s[i];
	      break;
	    }
	}

      if (i == s.size ())
	{
	  ans = 0;
	}
      else
	{
	  int cj = 0, jc = 0;
	  for (int j = i + 1; j < s.size (); j++)
	    {
	      if (prev == 'C' && s[j] == 'J')
		{
		  cj++;
		  prev = 'J';
		}
	      else if (prev == 'J' && s[j] == 'C')
		{
		  jc++;
		  prev = 'C';
		}
	    }
	  ans = x * cj + y * jc;
	}
    print_ans:
      cout << "Case #" << tt << ": " << ans << '\n';
    }


  return 0;
}
