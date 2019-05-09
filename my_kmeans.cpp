#include <bits/stdc++.h>

using namespace std;

template <typename _T>
pair<vector<int>, vector<vector<_T>>> do_kmeans(const vector<vector<_T>> &points, int num_centers, int epoh = 0)
{
    int epoh_max = epoh;
    int dim = points[0].size();
    vector<vector<_T>> centers(num_centers, vector<_T>(dim, 0));
    pair<vector<int>, vector<vector<_T>>> result{};

    const function<double(vector<_T>, vector<_T>)>
        calc_dist = [&](vector<_T> point1, vector<_T> point2) -> double {
        double dist = 0;
        assert(point1.size() == point2.size());
        for (int i = 0; i < point1.size(); i++)
        {
            dist += (point1[i] - point2[i]) * (point1[i] - point2[i]);
        }
        return dist;
    };

    //init centers
    srand((int)time(NULL));
    vector<array<double, 2>> range(dim, array<double, 2>{0, 0});
    for (int i = 0; i < dim; i++)
    {
        range[i] = array<double, 2>{points[0][i], points[0][i]};
        for (int j = 0; j < points.size(); j++)
        {
            if (points[j][i] > range[i][1])
                range[i][1] = points[j][i];
            if (points[j][i] < range[i][0])
                range[i][0] = points[j][i];
        }
    }
    function<void(void)> init_param = [&](){
        
        for (int i = 0; i < num_centers; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                // double m1 = (double)(rand() % 10000001) / 10000001;
                // centers[i][j] = m1 * (range[j][1] - range[j][0]) + range[j][0];
                set<int> st;
                assert(num_centers <= points.size());
                for(int i = 0; i < num_centers; i++){
                    int m0 = rand() % points.size();
                    if(find(st.begin(), st.end(), m0) != st.end()) i--;
                    else{
                        st.insert(m0);
                        centers[i] = points[m0];
                    }
                }
            }
        }
    };
    init_param();

    //iteration
    while (1)
    {
        result.first.clear();
        // classify
        for (int i = 0; i < points.size(); i++)
        {
            int min = -1;
            int index = 0;
            for (int j = 0; j < num_centers; j++)
            {
                int dist = calc_dist(centers[j], points[i]);
                if (min == -1 || dist < min)
                {
                    min = dist;
                    index = j;
                }
            }
            result.first.push_back(index);
        }
        // calc new centers
        vector<vector<double>> centers_zero(num_centers, vector<double>(dim, 0));
        vector<vector<double>> tmp = centers;
        centers = centers_zero;

        vector<int> cnt(num_centers, 0);
        for (int i = 0; i < result.first.size(); i++)
        {
            cnt[result.first[i]]++;
            for (int j = 0; j < dim; j++)
            {
                centers[result.first[i]][j] += points[i][j];
            }
        }
        for (int i = 0; i < num_centers; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                centers[i][j] = centers[i][j] / cnt[i];
                if (cnt[i] == 0){
                    // centers[i] = tmp[i];
                    init_param();
                    epoh = epoh_max;
                }  
            }
        }
        if (epoh == 0)
            break;
        epoh--;
    }
    result.second = centers;
    return result;
}
