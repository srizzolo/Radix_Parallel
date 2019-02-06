vector<int> cSort(vector<int> v, int n, int r)
{
    int count[r + 1];
    memset(count, 0, sizeof(count)); 

    for(int j=0; j<n; j++)
    {
       count[v[j]]++;
    }
    
    for(int j=1; j<=r; j++)
    {
        count[j] += count[j-1];
    }
    
    vector<int> fin(0);
    if(true) //Later change for ascending vs descending sort
    {
        int curr = count[0];

        for(int j=0; j<r; j++)
        {
            int diff = count[j] - curr;
            
            if(diff > 0 && (curr != 0))
            {
                do{
                    fin.push_back(j);
                    cout << j << " ";
                    diff--;
                }while(diff > 0);
            }
            curr = count[j];
        }
        cout << endl;
    }
    else //Sort descendingly
    {
        
    }
    return fin;
}