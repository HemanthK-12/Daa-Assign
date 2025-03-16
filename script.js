let moduleLoaded = false;

// Initialize the Module object if it doesn't exist
if (typeof Module === 'undefined') {
    var Module = {
        onRuntimeInitialized: function() {
            moduleLoaded = true;
            console.log('WebAssembly Module initialized');
        }
    };
} else {
    Module.onRuntimeInitialized = function() {
        moduleLoaded = true;
        console.log('WebAssembly Module initialized');
    };
}

function submit() {
    if (!moduleLoaded) {
        document.getElementById('answer').textContent = 'Wait for WebAssembly to initialize...';
        return;
    }

    const input = document.getElementById('input').value;
    try {
        // Parse input string
        const lines = input.trim().split('\n');
        if (lines.length < 2) {
            throw new Error('Invalid input format');
        }

        const [n, m] = lines[0].trim().split(' ').map(Number);
        
        // Validate n and m
        if (isNaN(n) || isNaN(m) || n <= 0 || m <= 0 || n > 1000) {
            throw new Error('Invalid n or m values');
        }

        // Validate number of edge lines
        if (lines.length < m + 1) {
            throw new Error('Not enough edges provided');
        }

        // Create flat array of edges with validation
        const edges = [];
        for (let i = 0; i < m; i++) {
            const [u, v] = lines[i + 1].trim().split(' ').map(Number);
            
            // Validate edge vertices
            if (isNaN(u) || isNaN(v) || u <= 0 || v <= 0 || u > n || v > n) {
                throw new Error(`Invalid edge: ${lines[i + 1]}`);
            }
            
            edges.push(u);
            edges.push(v);
        }

        // Call the CLIQUES function
        const result = Module.ccall(
            'CLIQUES',
            'string',
            ['number', 'array', 'number'],
            [n, edges, edges.length]
        );
        
        if (!result) {
            throw new Error('No result returned from CLIQUES');
        }

        document.getElementById('answer').textContent = result;
        console.log(result);
    } catch (error) {
        console.error('Error:', error);
        document.getElementById('answer').textContent = 'Error: ' + error.message;
    }
}