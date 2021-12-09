function rank = pagerank(ConnectivityMatrix)

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
rank = ones(dimension, 1);
for i = 1:100
    rank = TransitionMatrix * rank; 
end

%Return PageRank Vector with final ranks of each node
rank = rank / sum(rank);

end