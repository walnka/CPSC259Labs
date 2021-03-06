%% Calculate Page Rank
% Define ConnectivityMatrix
ConnectivityMatrix = [ 0 1 1 1 1; 1 0 0 1 0; 0 0 0 0 0; 1 1 1 0 0; 0 1 0 1 0] ;

%Get Dimension and sums of all columns of ConnectivityMatrix
dimension = size(ConnectivityMatrix, 1);
columnsums = sum(ConnectivityMatrix, 1);

%Define Random Walk Probability Factor as 0.85
p = 0.85;

%Find columns with 0 links to the node in it
zerocolumns = find(columnsums~=0);

%Create Sparse matrix and stochastic matrix using it
D = sparse( zerocolumns, zerocolumns, 1./columnsums(zerocolumns), dimension, dimension);
StochasticMatrix = ConnectivityMatrix * D;

%Find specific columns with 0 sum and transform stochasticMatrix
%accordingly
[row, column] = find(columnsums==0);
StochasticMatrix(:, column) = 1./dimension;

%Transform TransitionMatrix with StochasticMatrix to get proper ranking
%matrix
Q = ones(dimension, dimension);
TransitionMatrix = p * StochasticMatrix + (1 - p) * (Q/dimension);

%Define PageRank vector and iterate using transition matrix until converges
%(chose 100 as iteration time as it is plenty long enough to converge)
PageRank = ones(dimension, 1);
for i = 1:100
    PageRank = TransitionMatrix * PageRank; 
end

%Return PageRank Vector with final ranks of each node
PageRank = PageRank / sum(PageRank)

%{ 
Node Planning
0 1 1 1 1
1 0 0 1 0
0 0 0 0 0
1 1 1 0 0
0 1 0 1 0

    Nodes 1-5 Inclusive
    No node contains links to itself
    Node 1 contains links to all of the other nodes
    Node 2 contains links to node 1 and node 4
    Node 3 contains no links
    Node 4 contains links to nodes 1, 2, and 3
    Node 5 contains links to nodes 2 and 4.
%}