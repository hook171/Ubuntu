// Функция для получения текущей температуры
async function fetchCurrentTemperature() {
    try {
        const response = await fetch('/current');
        const data = await response.json();

        if (data.error) {
            document.getElementById('currentTemp').textContent = 'Error loading data';
        } else {
            document.getElementById('currentTemp').textContent = `${data.temperature} °C`;
        }
    } catch (error) {
        console.error('Error fetching current temperature:', error);
        document.getElementById('currentTemp').textContent = 'Error loading data';
    }
}

// Функция для получения статистики за период
async function fetchStatistics() {
    const start = document.getElementById('start').value;
    const end = document.getElementById('end').value;

    if (!start || !end) {
        alert('Please select both start and end dates.');
        return;
    }

    try {
        const response = await fetch(`/stats?start=${start}&end=${end}`);
        const data = await response.json();

        if (data.error) {
            alert(data.error);
        } else {
            document.getElementById('minTemp').textContent = data.min;
            document.getElementById('maxTemp').textContent = data.max;
            document.getElementById('avgTemp').textContent = data.avg;
        }
    } catch (error) {
        console.error('Error fetching statistics:', error);
        alert('Failed to fetch statistics');
    }
}

// Обновляем текущую температуру каждые 5 секунд
setInterval(fetchCurrentTemperature, 5000);

// Загружаем текущую температуру при запуске
fetchCurrentTemperature();

// Назначаем обработчик на кнопку для получения статистики
document.getElementById('fetchStatsButton').addEventListener('click', fetchStatistics);