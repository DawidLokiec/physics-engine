kernel void calcAccelerations(
    global const float* masses,
    global const float* positions,
    global float* accelerations,
    const ulong numBodies,
    const float softeningFactorSquared
) {
    const size_t threadIndex           = get_global_id(0);
	const size_t xCoordinateIndexBody1 = threadIndex * 3;
 	const size_t yCoordinateIndexBody1 = xCoordinateIndexBody1 + 1;
	const size_t zCoordinateIndexBody1 = xCoordinateIndexBody1 + 2;   

    float forceVector[3] = {0.0f, 0.0f, 0.0f};
    for (size_t i = 0; (i < numBodies) && (threadIndex < numBodies); ++i) {
        if (threadIndex != i) {
            const size_t xCoordinateIndexBody2 = i * 3;
            const size_t yCoordinateIndexBody2 = xCoordinateIndexBody2 + 1;
            const size_t zCoordinateIndexBody2 = xCoordinateIndexBody2 + 2;     
           
            const float distanceVectorXCoordinate = positions[xCoordinateIndexBody1] - positions[xCoordinateIndexBody2];
            const float distanceVectorYCoordinate = positions[yCoordinateIndexBody1] - positions[yCoordinateIndexBody2];
			const float distanceVectorZCoordinate = positions[zCoordinateIndexBody1] - positions[zCoordinateIndexBody2];
            
            const float distance = sqrt(
                (distanceVectorXCoordinate * distanceVectorXCoordinate) + 
                (distanceVectorYCoordinate * distanceVectorYCoordinate) +
                (distanceVectorZCoordinate * distanceVectorZCoordinate)
            ) + softeningFactorSquared; // to avoid zero in the following divisions 
            
            const float normalizedDistanceVectorXCoordinate = distanceVectorXCoordinate / distance;
			const float normalizedDistanceVectorYCoordinate = distanceVectorYCoordinate / distance;
			const float normalizedDistanceVectorZCoordinate = distanceVectorZCoordinate / distance;
            const float receivedForce = masses[i] / (distance * distance);
            // It is possible to write directly in to the accelerations array here, but the false sharing will
			// reduce in this loop significantly the performance
			forceVector[0] += (receivedForce * normalizedDistanceVectorXCoordinate);
			forceVector[1] += (receivedForce * normalizedDistanceVectorYCoordinate);
			forceVector[2] += (receivedForce * normalizedDistanceVectorZCoordinate);
            
        }
    }
    // false sharing is ok here
	accelerations[xCoordinateIndexBody1] = 6.67430e-11 * forceVector[0];
	accelerations[yCoordinateIndexBody1] = 6.67430e-11 * forceVector[1];
	accelerations[zCoordinateIndexBody1] = 6.67430e-11 * forceVector[2];
}
